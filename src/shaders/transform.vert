#version 330 core
out vec4 vertPos;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 transform;
out vec3 Normal;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    vertPos = gl_Position;
    Normal = aNorm;
} 