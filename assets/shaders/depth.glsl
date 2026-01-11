#shader vertex
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightViewProjection;

void main()
{
    gl_Position = lightViewProjection * model * vec4(aPos, 1.0);
}

#shader fragment
#version 460 core
void main() {}
