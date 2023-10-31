#version 330 core
out vec4 FragColor;
in vec4 vertPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform mat4 modelMatrix;

void main()
{
	//	Convert normals to world coordinates
	vec3 realNormal = transpose(inverse(mat3(modelMatrix))) * Normal;

	//	Light information
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);	//	White light
	vec4 lightPos = vec4(1.0f, 5.0f, 2.0f, 1.0f);

	//	Ambient light
	float ambientIntensity = 0.5f;	//	Out of 1.0f
	vec3 ambient = ambientIntensity * lightColor;
	vec3 result = ambient * objectColor;

	//	Diffuse light
	vec3 norm = normalize(realNormal);
	vec3 lightDir = normalize(vec3(lightPos) - vec3(vertPos));
	float diffuseFactor = max(0, dot(norm,lightDir));
	vec3 diffuse = diffuseFactor * lightColor;

	vec3 finalResult = (result + diffuse) * objectColor;

	FragColor = vec4(finalResult, 1.0f);
}