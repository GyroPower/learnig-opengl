#version 330 core 

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform float ambientStrength;
uniform float shininess;
uniform vec3 lightColor;

void main(){

	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 cameraDir = normlize(cameraPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + cameraDir);

	vec3 normal = normalize(Normal);

	vec3 ambient = ambientStrength * lightColor;
	vec3 result = ambient * objectColor;

	float diff = max(dot(normal,lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	result += diffuse;

	float spec = pow(max(dot(normal,halfwayDir), 0.0), shininess);

	vec3 specular = lightColor * spec;
	result += specular;

	FragColor = vec4(result, 1.0);
}