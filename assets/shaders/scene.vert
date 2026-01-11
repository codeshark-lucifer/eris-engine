#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec4 FragPosLightSpace[16];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightViewProjection[16];
uniform int lightCount;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;

    for(int i = 0; i < lightCount; i++) {
        FragPosLightSpace[i] = lightViewProjection[i] * worldPos;
    }

    gl_Position = projection * view * worldPos;
}
