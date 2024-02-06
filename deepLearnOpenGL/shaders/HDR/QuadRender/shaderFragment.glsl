#version 330 core 

out vec4 FragColor; 

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;
uniform bool hdr;

void main(){

	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	vec3 result = vec3(0.0);
	//This is a exposure tone mapping algorithm 
	//This with the exposure uniform give us to control if give more 
	//detail in dark or bright regions of the hdr values

	
	if (hdr){
		
		//result = hdrColor / (hdrColor + vec3(1.0));

		result = vec3(1.0) - exp(-hdrColor * exposure);
		
		//Use of gamma correction
		result = pow(result, vec3(1.0 / gamma));
	}
	else
		//Use of gamma correction
		result = pow(hdrColor, vec3(1.0/gamma));
  
    FragColor = vec4(result, 1.0);
	

}