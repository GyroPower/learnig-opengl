#version 330 core 

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D depthMap;
uniform float plane_near;
uniform float plane_far;

float LinearizeDepth(float depth){
	float z = depth * 2.0 - 1.0;
	return (2.0 * plane_near * plane_far) / (plane_far + plane_near - z * (plane_far - plane_near));
}

void main(){
	
	float depthValue = texture(depthMap, TexCoord).r;

	FragColor = vec4(vec3(depthValue), 1.0);

}