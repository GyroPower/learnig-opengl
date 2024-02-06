#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentNormal;
	vec3 TangentViewPos;
	vec3 TangentLightPos;
	vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform vec3 lightColor;

void main(){
	
	
	
	vec3 color = texture(diffuseMap,fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.TangentNormal);


	vec3 ambient = 0.0 * color;

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	


	float diff = max(dot(lightDir,normal),0.0);


	vec3 diffuse = lightColor * diff ;

	color *= diffuse;


	float distance = length(fs_in.TangentFragPos - fs_in.TangentLightPos);
	
	color *= 1.0 / (distance * distance);
	
	
	
	FragColor = vec4(ambient + color, 1.0);
	
}