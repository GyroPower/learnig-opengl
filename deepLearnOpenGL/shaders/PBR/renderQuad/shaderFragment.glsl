#version 330 core 

out vec2 FragColor;

uniform sampler2D LUTTexture;

in vec2 TexCoords;

void main(){

	FragColor = texture(LUTTexture,TexCoords).rg;
}