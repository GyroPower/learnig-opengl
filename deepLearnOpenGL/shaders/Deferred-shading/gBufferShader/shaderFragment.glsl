#version 330 core 

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main(){
	
	//Store the pos vector as a color in the firts gbuffer texture
	gPosition = FragPos;

	//the same with the normal vec with the second gbuffer texture
	gNormal = normalize(Normal);

	//same as the others before, just with this one we store the albedo in the rgb comp
	//of the vec4 and the spec value in the alpha value of the vec4
	gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
	gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}