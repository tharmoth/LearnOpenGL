#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

/**********************************************************************************************************************
 * Directional Light Calculations
 * See: https://learnopengl.com/Lighting/Light-casters
 **********************************************************************************************************************/
struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directionalLight;
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));

    // diffuse
    vec3  lightDir = normalize(-light.direction);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, UV));

    // specular
    vec3  halfwayDir = normalize(lightDir + viewDir);
    float spec       = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3  specular   = light.specular * spec * vec3(texture(material.specular, UV));

    return ambient + diffuse + specular;
}

/**********************************************************************************************************************
 * Point Light Calculations
 * See: https://learnopengl.com/Lighting/Light-casters
 **********************************************************************************************************************/
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));

    // diffuse
    vec3  lightDir = normalize(light.position - fragPos);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, UV));

    // specular
    vec3  halfwayDir = normalize(lightDir + viewDir);
    float spec       = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3  specular   = light.specular * spec * vec3(texture(material.specular, UV));

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

/**********************************************************************************************************************
 * Spot Light Calculations
 * See: https://learnopengl.com/Lighting/Light-casters
 **********************************************************************************************************************/
struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float innerCutoff;
    float outerCutoff;
};
uniform SpotLight spotLight;
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3  lightDir  = normalize(light.position - fragPos);
    float theta     = acos(dot(normalize(light.position - fragPos), normalize(-light.direction)));
    float epsilon   = light.innerCutoff - light.outerCutoff;
    float intensity = (theta - light.outerCutoff) / epsilon;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    if(theta < light.outerCutoff) {
        // ambient
        ambient = light.ambient * vec3(texture(material.diffuse, UV));

        // diffuse
        vec3  lightDir = normalize(light.position - fragPos);
        float diff     = max(dot(normal, lightDir), 0.0);
        diffuse        = light.diffuse * diff * vec3(texture(material.diffuse, UV));

        // specular
        vec3  halfwayDir = normalize(lightDir + viewDir);
        float spec       = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        specular         = light.specular * spec * vec3(texture(material.specular, UV));

        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        // Apply the blend between inner and outer radius
        diffuse  *= intensity;
        specular *= intensity;
    }

    return ambient + diffuse + specular;
}


void main()
{
    // Calculate light position indepentant variables
    vec3 normal  = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    // Phase 1: Add Directional Light
    vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDir);
    // Phase 2: Add Point Lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalculatePointLight(pointLights[i], normal, viewDir, FragPos);
    }
    // Phase 3: Add Spot Light
    result += CalculateSpotLight(spotLight, normal, viewDir, FragPos);

    FragColor = vec4(result, 1.0);
}