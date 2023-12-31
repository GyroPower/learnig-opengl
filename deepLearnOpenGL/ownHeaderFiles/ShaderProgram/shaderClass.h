#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


class Shader {

public:
	unsigned int ID;

	/*Shader(const char* VertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;*/

	Shader(const char* VertexPath, const char* fragmentPath) {
		//1. retrieve the vertex/fragment source code from filepath
		std::string vertexCode;
		std::string fragmentCode;
		
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		
		// ensure ifstream objects can throw exceptions:  
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		
		try {
			//open files
			vShaderFile.open(VertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream,gShaderStream;
			//read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch(std::ifstream::failure e){
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;

		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILES\n" << infoLog << std::endl;
		}


		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);



		glLinkProgram(this->ID);

		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader(const char* VertexPath, const char* fragmentPath, const char* geometryPath ) {
		//1. retrieve the vertex/fragment source code from filepath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		// ensure ifstream objects can throw exceptions:  
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			//open files
			vShaderFile.open(VertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream, gShaderStream;
			//read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();


			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			
			gShaderFile.open(geometryPath);
			gShaderStream << gShaderFile.rdbuf();

			gShaderFile.close();
			geometryCode = gShaderStream.str();
			
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		const char* gShaderCode = geometryCode.c_str();;


		unsigned int vertex, fragment, geometry;

		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILES\n" << infoLog << std::endl;
		}

		
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILES\n" << infoLog << std::endl;
		}
		

		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);

		glAttachShader(this->ID, geometry);
		



		glLinkProgram(this->ID);

		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geometry);

		
	}

	void use() {
		glUseProgram(this->ID);
	}

	void setBool(const std::string& name, bool value) const{
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value)const {
		glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
	}

	void setMat4(const std::string& name, glm::mat4 value)const {
		int mat4Loc = glGetUniformLocation(this->ID, name.c_str());
		glUniformMatrix4fv(mat4Loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void setVec3(const std::string& name, glm::vec3 value)const {
		int vec3Loc = glGetUniformLocation(this->ID, name.c_str());
		
		glUniform3f(vec3Loc, value.x, value.y, value.z);
	}

	void setVec2(const std::string& name, glm::vec2 value)const {
		int vec2Loc = glGetUniformLocation(this->ID, name.c_str());
		glUniform2f(vec2Loc, value.x, value.y);
	}
};