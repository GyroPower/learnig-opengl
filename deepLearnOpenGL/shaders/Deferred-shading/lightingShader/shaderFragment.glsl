#version 330 core 

out vec4 FragColor; 

in vec2 TexCoords;

// the three rendered scenes with the actual data of the scene
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
	float Radius;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main(){

	vec3 FragPos = texture(gPosition, TexCoords).rgb;

	vec3 Normal = texture(gNormal, TexCoords).rgb;

	vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;

	float Specular = texture(gAlbedoSpec, TexCoords).a;

	vec3 lighting = Albedo * 0.1;
	vec3 viewDir = normalize(viewPos - FragPos);

	for (int i = 0; i < NR_LIGHTS; i++){
		
		float distance = length(lights[i].Position - FragPos);

		// this is how it should work lighting it a way we could have a lot of points lights
		// if it's in the radius of the light it should make the calc
		// but how the gpu and glsl works it's not the actual implementation
		if (distance < lights[i].Radius){
			
			// Do the Blinn-phong calcg
			vec3 lightDir = normalize(lights[i].Position - FragPos);
			vec3 diffuse = max(dot(Normal,lightDir), 0.0) * Albedo * lights[i].Color;
			vec3 halfwayDir = normalize(lightDir + viewDir);

			float spec = pow(max(dot(Normal,halfwayDir),0.0), 16.0);
			vec3 specular = lights[i].Color * spec * Specular;

		
			float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance); 

			diffuse *= attenuation;
			specular *= attenuation;

			lighting += diffuse + specular;
		
		}
	}
	FragColor = vec4(lighting, 1.0);
}
