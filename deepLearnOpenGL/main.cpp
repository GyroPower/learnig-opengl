#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shaderClass.h"
#include"stb_image.h"
#include<cmath>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>



void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window,float &move,Shader shaderProgramInstance,bool &pressed) {
	
	float timeValue = glfwGetTime();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
		glfwSetWindowShouldClose(window, true);
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
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,1.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f,1.0f,
	};

	//float texCoords[] = {
	//	0.0f,0.0f,
	//	1.0f,0.0f,
	//	0.5f,1.0f
	//};


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	
	unsigned int texture1,texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
		0,1,3,
		1,2,3
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
	
	glBindVertexArray(VAO);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5f));

	
	float move = 0.1f;
	bool pressed = false;
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		
		unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

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

		glm::mat4 trans2 = glm::mat4(1.0f);

		trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));

		trans2 = glm::rotate(trans2, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans2));
		
		
		glm::mat4 trans3 = glm::mat4(1.0f);
		trans3 = glm::translate(trans3, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans3 = glm::scale(trans3, (float)sin(glfwGetTime()) * glm::vec3(1.5f, 1.5f, 1.0f));
		transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans3));
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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