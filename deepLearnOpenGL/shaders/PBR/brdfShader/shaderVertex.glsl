#version 330 core 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 WorldPos;
out vec2 TexCoords;

void main(){

	WorldPos = aPos;
	TexCoords = aTexCoords;
	gl_Position = vec4(WorldPos,1.0);
}