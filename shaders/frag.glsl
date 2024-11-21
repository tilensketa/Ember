#version 430 core

out vec4 FragColor;

in vec3 color;
in vec3 Normal;
in vec3 crntPos;

vec4 lightColor = vec4(1,1,1,1);
vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);

uniform vec3 camPos;

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 0.5f, 2.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return ((diffuse + ambient) + specular) * lightColor;
}

void main()
{
	FragColor = direcLight() * vec4(color, 1.0f);
  // FragColor = vec4(Normal, 1.0f);
}
