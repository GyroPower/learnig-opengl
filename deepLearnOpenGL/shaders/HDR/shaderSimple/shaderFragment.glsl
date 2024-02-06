#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform sampler2D diffuseMap;
uniform vec3 lightColor[4];
uniform vec3 lightPos[4];

void main(){
	vec3 color = texture(diffuseMap,fs_in.TexCoords).rgb;
	
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.0 * color;

	vec3 lighting = vec3(0.0);

	for (int i = 0; i < 4; i++){
		vec3 lightDir = normalize(lightPos[i] - fs_in.FragPos);

		float diff = max(dot(lightDir,normal), 0.0);
	
		vec3 diffuse = lightColor[i] * diff * color;
		vec3 result = diffuse;

		float distance = length(fs_in.FragPos - lightPos[i]);
		result *= 1.0 / (distance * distance);

		lighting += result;
	}

	
	FragColor = vec4(ambient + lighting, 1.0);
}