#version 330 core
out vec4 worldPos;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    worldPos = modelMatrix * vec4(aPos, 1.0f);
    gl_Position = vpMatrix * worldPos;
    Normal = (modelMatrix * vec4(aNorm, 0)).xyz;
    TexCoord = aTexCoord;
}