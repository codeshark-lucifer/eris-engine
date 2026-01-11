#shader vertex
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 460 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D depthMap;

void main()
{
    float d = texture(depthMap, TexCoord).r;
    FragColor = vec4(vec3(d), 1.0);
}
