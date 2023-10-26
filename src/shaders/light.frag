#version 330 core
out vec4 FragColor;
in vec4 vertPos;
in vec3 Normal;

uniform vec3 objectColor;

void main()
{
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);	//	White light
	vec4 lightPos = vec4(1.0f, 5.0f, 2.0f, 1.0f);
	vec4 furthestFromLight = vec4(-1.0f, -1.0f, 1.0f, 1.0f);	//	Bottom-left far corner of clip space
	float maxDistance = distance(lightPos, furthestFromLight);

	//	Ambient light
	float ambientIntensity = 0.5f;	//	Out of 1.0f
	vec3 ambient = ambientIntensity * lightColor;
	vec3 result = ambient * objectColor;

	//	Distance light
	//float distToLight = distance(vertPos,lightPos);
	//vec3 finalResult = (result * distToLight) / maxDistance;

	//	Diffuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3(lightPos) - vec3(vertPos));
	float diffuseFactor = max(0, dot(norm,lightDir));
	vec3 diffuse = diffuseFactor * lightColor;

	vec3 finalResult = (result + diffuse) * objectColor;

	FragColor = vec4(finalResult, 1.0f);
}