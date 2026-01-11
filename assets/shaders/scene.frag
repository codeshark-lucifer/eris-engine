#version 460 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace[16];

out vec4 FragColor;

uniform sampler2D diffuse_texture1;
uniform sampler2D shadowMaps[16];
uniform vec3 viewPos;

struct Light {
    int type; // 0 = Dir, 1 = Point, 2 = Spot
    vec3 position;
    vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    bool castsShadow;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform Light lights[16];
uniform int lightCount;

vec3 CalculateDirectionalLight(vec3 norm, vec3 viewDir, Light light)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = material.ambient * light.color;
    vec3 diffuse  = material.diffuse * diff * light.color;
    vec3 specular = material.specular * spec * light.color;

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(vec3 norm, vec3 viewDir, Light light, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * distance * distance);

    vec3 ambient  = material.ambient * light.color * attenuation;
    vec3 diffuse  = material.diffuse * diff * light.color * attenuation;
    vec3 specular = material.specular * spec * light.color * attenuation;

    return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(vec3 norm, vec3 viewDir, Light light, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * distance * distance);

    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = max(light.cutOff - light.outerCutOff, 0.001); // prevent divide by zero
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = material.ambient * light.color * attenuation * intensity;
    vec3 diffuse  = material.diffuse * diff * light.color * attenuation * intensity;
    vec3 specular = material.specular * spec * light.color * attenuation * intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 albedo = texture(diffuse_texture1, TexCoord).rgb;

    // Start with ambient color
    vec3 result = material.ambient * albedo;

    // Loop through all lights
    for(int i = 0; i < lightCount; i++)
    {
        Light light = lights[i];
        vec3 contribution = vec3(0.0);

        if(light.type == 0) // Directional
        {
            contribution = CalculateDirectionalLight(norm, viewDir, light);
        }
        else if(light.type == 1) // Point
        {
            contribution = CalculatePointLight(norm, viewDir, light, FragPos);
        }
        else if(light.type == 2) // Spot
        {
            contribution = CalculateSpotLight(norm, viewDir, light, FragPos);
        }

        result += contribution;
    }

    FragColor = vec4(result, 1.0);
}
