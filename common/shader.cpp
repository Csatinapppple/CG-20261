#include <shader.hpp>
#include <fileReader.hpp>

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void handleId(unsigned int id, GLenum pname) {
	int success;
	char infoLog[512];

	switch (pname) {
		case GL_LINK_STATUS:
			glGetProgramiv(id, pname, &success);
			if (!success) {
				glGetProgramInfoLog(id, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			break;
		case GL_COMPILE_STATUS:
			glGetShaderiv(id, pname, &success);
			if (!success) {
				glGetShaderInfoLog(id, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
			break;
	}
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode = read_file(vertexPath);
	std::string fragmentCode = read_file(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	
	unsigned int vertex, fragment;
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	handleId(vertex, GL_COMPILE_STATUS);
	
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	handleId(fragment, GL_COMPILE_STATUS);
	
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	handleId(ID, GL_LINK_STATUS);
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}
