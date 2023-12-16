#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;


uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LinearDepth(float depth){
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main(){

	//float depth = LinearDepth(gl_FragCoord.z) / far;
	float ratio = 1.00/ 1.52;
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I,normalize(Normal),ratio);

	FragColor = vec4(texture(skybox,R).rgb,1.0);

}