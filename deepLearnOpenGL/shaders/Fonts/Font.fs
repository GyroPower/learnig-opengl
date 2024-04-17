#version 420 core 

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text; //we use the r color for the alpha value
uniform vec3 textColor;

void main(){
    //where are grayscale values of 1.0 are gonna render for the blending 
    vec4 sampled = vec4(1.0,1.0,1.0,texture(text,TexCoords).r);
    //and the actual color multiplied by the text and the uniform color
    color = vec4(textColor,1.0) * sampled;
}
