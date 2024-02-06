#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

//This shader program was made to see if i was able to made the same result like
//the shader program called "shaderSimple" but with tangent space calc, and only use
//one quad struct, but it turns out that it looks brighter than render a quad and scale
//it in his z component and inverse his normals, but it works with the tone mapping
//that use a exposure parameter, it turns down the brightness a lot and make darker
//far away plces of the tunnel scene

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentNormal;
	vec3 TangentViewPos;
	vec3 TangentLightPos;
	vec3 TangentFragPos;

} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 viewPos;
uniform vec3 lightPos;


void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	mat3 normalMatrix = transpose(inverse(mat3(model)));

	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	vec3 B = normalize(normalMatrix * aBitangent);

	mat3 TBN = transpose(mat3(T,B,N));

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	vs_out.TangentNormal = TBN * aNormal;
	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;
}