#version 330 core 

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// tile noise texture over screen, based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(800/4.0, 600/4.0);

void main(){
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;

	//This compares differents samples nearby the fragments pos 
	//compares the depth value and if it's nearby the actual fragment and 
	//it's depth value it's greater than the actual compared sample
	//it means it has to have a greater occlusion value, because light 
	//has no great impact because there are fragments in front of that actual fragment
	for (int i = 0; i < kernelSize; ++i ){

		//Transform the sample in view-space
		vec3 samplePos = TBN * samples[i];
		samplePos = fragPos + samplePos * radius;
		
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;
		
		float sampleDepth = texture(gPosition, offset.xy).z;
		//This ensures that the samples values are close in the radius
		//and the ones that are far away don't influence the occlusion value
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		//checks if the fragment depth it's highter than the sample
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0: 0.0) * rangeCheck;
	}
	//normalize the occlusion contibution by the size of the kernel
	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;
	
}