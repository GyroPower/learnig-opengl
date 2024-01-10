#version 330 core 

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec3 LightPos[4];
uniform vec3 cameraPos;

uniform float ambientStrength;
uniform float shininess;
uniform vec3 LightColor[4];
uniform bool gamma;
uniform bool attenuation_bool_quad;


vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor){
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 cameraDir = normalize(cameraPos - fragPos);
	
	vec3 ambient = 0.05 * lightColor;

	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuse = diff * lightColor;

	vec3 halfwayDir = normalize(lightDir + cameraDir);

	float spec = pow(max(dot(normal,halfwayDir),0.0),64.0);

	vec3 specular = spec * lightColor;

	float distance = length(lightPos - fragPos);
	
	float attenuation = 0.0;

	//attenuation = 1.0 / (distance * distance);

	if (attenuation_bool_quad){
		attenuation = 1.0 / (distance * distance);
	}
	else{
		attenuation = 1.0 / distance;
	}


	diffuse *= attenuation;
	specular *= attenuation;
	ambient *= attenuation;

	return diffuse + specular ;
}

void main(){

	
	vec3 color = texture(texture1,TexCoords).rgb;
	
	vec3 lighting = vec3(0.0);

	for (unsigned int i = 0; i < 4; i++){
		lighting += BlinnPhong(normalize(Normal), FragPos, LightPos[i], LightColor[i]);
		
	}


	color *= lighting;

	if (gamma)
		color = pow(color,vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}