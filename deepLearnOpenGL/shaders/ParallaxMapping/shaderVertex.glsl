#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main(){

	gl_Position = projection * view * model * vec4(aPos,1.0);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 N = normalize(mat3(model) * aNormal);
    //T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(mat3(model) * aBitangent);

	//getting the inverse
	mat3 TBN = transpose(mat3(T,B,N));

	//Transforming the necesary coords for lit in TBN coord system
	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;


}