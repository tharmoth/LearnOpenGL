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

void main()
{
    // Calculate light position indepentant variables
    vec3 normal  = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    // Phase 1: Add Directional Light
    vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDir);

    FragColor = vec4(result, 1.0);
}
