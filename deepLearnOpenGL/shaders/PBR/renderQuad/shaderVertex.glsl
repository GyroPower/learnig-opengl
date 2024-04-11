#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 Pos;
out vec2 TexCoords;

void main(){
	Pos = aPos;
	TexCoords = aTexCoords;
	gl_Position = vec4(Pos,1.0);


}