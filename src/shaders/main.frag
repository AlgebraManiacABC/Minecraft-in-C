#version 330 core

in vec4 worldPos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D tex;
uniform mat4 modelMatrix;

vec4 light(vec4 texColor);

void main()
{
	vec4 texColor = texture(tex,TexCoord);
	vec4 lightColor = light(texColor);
	FragColor = lightColor;
	//FragColor.rgb = Normal;
	//FragColor.a = 1.0f;
}

vec4 light(vec4 texColor)
{
	//	Convert normals to world coordinates
	vec3 realNormal = transpose(inverse(mat3(modelMatrix))) * Normal;

	//	Light information
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);	//	White light
	//vec4 lightPos = vec4(1.0f, 500.0f, 2.0f, 1.0f);

	//	Ambient light
	float ambientIntensity = 0.5f;	//	Out of 1.0f
	vec3 ambient = ambientIntensity * lightColor;
	//vec3 intermediate = ambient * objectColor;
	vec3 intermediate = ambient * vec3(texColor);

	//	Diffuse light
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(vec3(lightPos) - vec3(worldPos));
	vec3 lightDir = vec3(0.0f, 1.0f, 0.0f);
	float diffuseFactor = max(0, dot(norm,lightDir));
	vec3 diffuse = diffuseFactor * lightColor;

	vec3 result = (intermediate + diffuse) * vec3(texColor);

	return vec4(result, 1.0f);
}