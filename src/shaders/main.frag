#version 330 core

out vec4 FragColor;
vec4 getTexture();
vec4 light(vec4 texColor);

void main()
{
	vec4 texColor = getTexture();
	vec4 lightColor = light(texColor);
	FragColor = lightColor;
}