#version 330 core

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

float ShadowCalculation(vec3 fragPos){
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(depthMap, fragToLight).r;

	closestDepth *= far_plane;

	float samples = 4.0;
	float offset = 0.1;

	float currentDepth = length(fragToLight);
	float bias = 0.05;
	float shadow = 0.0;

	for (float x = -offset; x < offset; x+=offset / (samples * 0.5)){
		for (float y = -offset; y < offset; y += offset / (samples * 0.5)){
			for (float z = -offset; z < offset; z += offset/(samples * 0.5)){
				closestDepth = texture(depthMap, fragToLight + vec3(x,y,z)).r;
				closestDepth *= far_plane;
				
				if (currentDepth - bias > closestDepth)
					shadow += 1.0;

				
			}
		}
	}
	shadow /= (samples * samples * samples);
	// FragColor = vec4(vec3(closestDepth/ far_plane),1.0);

	return shadow;

}

void main(){

	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = texture(normalMap,fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 lightColor = vec3(0.3);
	
	vec3 ambient = 0.3 * color;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = 0.0;

	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal,halfwayDir), 0.0), 128.0);

	vec3 specular = spec * lightColor;

	float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;

	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0);
}

