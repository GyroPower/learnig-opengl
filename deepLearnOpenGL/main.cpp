#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shaderClass.h"
#include"stb_image.h"
#include<cmath>

unsigned int DrawToTriangesNextToEachOther() {
	float Triangles[] = {
		-0.5f,0.0f,0.0f,
		 0.0f,0.5f,0.0f,
		 0.0f,0.0f,0.0f,

		 0.6f,0.0f,0.0f,
		 0.1f,0.0f,0.0f,
		 0.1f,0.5f,0.0f
	};

	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangles), Triangles, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return VAO;

}

struct TwoTrianges {
	unsigned int Triangle1,Triangle2;

};

struct TwoShaderPrograms {
	unsigned int Shader1, Shader2;
};

unsigned int vertexShader(const char* vertexSource) {

	unsigned int VertexShader;
	
	VertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(VertexShader, 1, &vertexSource, NULL);

	//See if there is an error of compilation of the vertexShader
	int success;
	char infoLog[512];

	//Compile the vertexShader
	glCompileShader(VertexShader);

	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	return VertexShader;
}

unsigned int FragmentShaderGen(const char* fragmentSource) {

	unsigned int FragmentShader;

	int success;
	char infoLog[512];


	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return FragmentShader;
}

unsigned int shaderProgramBuild(const char* vertexSource, const char* fragmentSource) {

	unsigned int ShaderProgram;
	int success;
	char infoLog[512];

	ShaderProgram = glCreateProgram();

	unsigned int VertexShader = vertexShader(vertexSource);
	unsigned int FragmentShader = FragmentShaderGen(fragmentSource);

	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);

	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(FragmentShader);
	glDeleteShader(VertexShader);

	return ShaderProgram;
}

TwoTrianges TwoTrianglesDifferents() {
	
	TwoTrianges Triangles;

	float Triangle1[] = {
		-0.5f,0.0f,0.0f,
		 0.0f,0.0f,0.0f,
		 0.0f,0.5f,0.0f
	};

	float triangle2[] = {
		//Vertices		 //Colors
		0.6f,0.0f,0.0f,  1.0f,0.0f,0.0f,
		0.1f,0.0f,0.0f,  0.0f,1.0f,0.0f,
		0.1f,0.5f,0.0f,  0.0f,0.0f,1.0f
	};

	//Triangle 1
	unsigned int VAO1;

	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	unsigned int VBO1;

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle1), Triangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Triangles.Triangle1 = VAO1;

	//Triangle 2
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	unsigned int VBO2;

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Triangles.Triangle2 = VAO2;

	return Triangles;

};

TwoShaderPrograms ShadersPrograms(const char *vertexSource1,const char *vertexSource2, const char* fragmentSource1, const char* fragmentSource2) {
	TwoShaderPrograms shadersPrograms;

	shadersPrograms.Shader1 = shaderProgramBuild(vertexSource1,fragmentSource1);
	shadersPrograms.Shader2 = shaderProgramBuild(vertexSource2,fragmentSource2);

	return shadersPrograms;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}




void processInput(GLFWwindow* window,float &move,Shader shaderProgramInstance,bool &pressed) {
	
	float timeValue = glfwGetTime();

	
	
	if (pressed && sin(timeValue) <= 0.0f) {
		pressed = false;
		std::cout << "to false" << std::endl;

	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !pressed) {
		if (move <= 1.0f) {
			move += 0.1f;
			pressed = true;
			std::cout << "pressed" << std::endl;

		}
		shaderProgramInstance.setFloat("move", move);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !pressed) {
		if (move > 0.0f) {
			move -= 0.1f;
			pressed = true;
			std::cout << "pressed" << std::endl;

		}
		shaderProgramInstance.setFloat("move", move);
	}
	
	

	
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	glViewport(0, 0, 800, 600);


	float vertices[] = {
		//Vertices		 //Colors				//TexCoords
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.3f,0.2f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.2f,0.2f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.2f,0.3f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 0.3f,0.3f,
	};

	float texCoords[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f
	};


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	
	unsigned int texture1,texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);
	

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);
	
	unsigned int indices[] = {
		0,3,2,
		0,1,2
	};

	/*
	int success;
	char infoLog[512];*/

	Shader shaderProgram("shaderVertex.glsl", "shaderFragment.glsl");

	//Creating vertex array object for read the data of the VBO's
	unsigned int VAO,EBO;
	glGenVertexArrays(1, &VAO);
	
	//creating vertex buffer object
	//gets the data for the primitive to render, this only have vertices coordinates
	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		

	//unsigned int Triangles = DrawToTriangesNextToEachOther();

	//TwoTrianges twoTriangles = TwoTrianglesDifferents();
	//TwoShaderPrograms twoShaders = ShadersPrograms(vertexShaderSource,anotherVertexShaderSource,FragmentShaderSource,FragmentShaderYellow);
	
	//glUseProgram(shaderProgram);

	/*std::cout << twoShaders.Shader1 << std::endl;
	std::cout << twoShaders.Shader2 << std::endl;*/

	/*int nrAttributes;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	*/

	shaderProgram.use();
	shaderProgram.setInt("texture1", 0);
	shaderProgram.setInt("texture2", 1);
	float move = 0.1f;
	bool pressed = false;
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		/*float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		int vertexColorLocation = glGetUniformLocation(twoShaders.Shader2, "ourColor");*/

		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glUseProgram(twoShaders.Shader1);
		//
		//glBindVertexArray(twoTriangles.Triangle1);
		//
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shaderProgram.use();
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//glUseProgram(twoShaders.Shader2);
		////glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//glBindVertexArray(twoTriangles.Triangle2);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
		processInput(window,move,shaderProgram,pressed);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;


}