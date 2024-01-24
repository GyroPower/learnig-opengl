#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;

} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculartion(vec4 fragPosLightSpace){
	
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closesDepth = texture(shadowMap,projCoords.xy).r;

	float currentDepth = projCoords.z;

	float shadow = currentDepth > closesDepth ? 1.0 : 0.0;

	return shadow;

}

void main(){
	vec3 color = texture(diffuseTexture, fs_in.TexCoord).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);

	vec3 ambient = 0.15 8 lightColor;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	float spec = 0.0;
	vec3 halfWayDir = normalize(lightDir + viewDir);

	spec = pow(max(dot(normal,halfWayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular) * color);

	FragColor = vec4(lighting, 1.0);
	//gl_FragDepth = gl_FragCoord.z
}
