#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 direction;
    vec3 position;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = acos(dot(normalize(light.position - FragPos), normalize(-light.direction)));
    float epsilon   = light.cutoff - light.outerCutoff;
    float intensity = (theta - light.outerCutoff) / epsilon;

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    if(theta < light.outerCutoff) {
        // ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));

        // diffuse
        vec3 norm = normalize(Normal);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, UV));

        // specular
        vec3 viewDir = normalize(-FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        diffuse  *= intensity;
        specular *= intensity;

        FragColor = vec4((ambient + diffuse + specular), 1.0);
    } else {
        FragColor = vec4(light.ambient * attenuation * vec3(texture(material.diffuse, UV)), 1.0f);
    }
//    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}