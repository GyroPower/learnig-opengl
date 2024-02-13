#version 330 core
//we have to especify to wich attachment we are gonna output the shader fragment 
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform sampler2D diffuseTexture;
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];
uniform bool renderCube;
uniform vec3 viewPos;


vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor);

void main(){
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;

	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.0 * color;

	vec3 lighting = vec3(0.0);
	
	for (int i = 0; i < 4; i++){
		lighting += BlinnPhong(normal, fs_in.FragPos, lightPos[i], lightColor[i]);
	}
	color *= ambient + lighting;

	FragColor = vec4(color, 1.0);
	
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	if(brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(vec3(0.0),1.0);
}


vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor){
	
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfWayDir = normalize(lightDir + viewDir);

	float diff = max(dot(lightDir, normal),0.0);
	float spec = pow(max(dot(normal, halfWayDir),0.0), 64.0);

	vec3 diffuse = diff * lightColor;
	vec3 specular = spec * lightColor;
		
	
	float distance = length(lightPos - fragPos);

	float attenuation = 1.0 / (distance * distance);
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}