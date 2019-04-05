#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

Shader::Shader(std::string vertex_path, std::string fragment_path) : Shader(vertex_path, fragment_path, "NO_PATH") {}

Shader::Shader(std::string vertex_path, std::string fragment_path, std::string geometry_path) {
	const GLchar* vertexShaderSource;
	const GLchar* fragmentShaderSource;
	const GLchar* geometryShaderSource;

	std::string vertexShaderCode;
	try {
		std::ifstream input(vertex_path);
		input.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		std::stringstream vertexCodeStream;
		vertexCodeStream << input.rdbuf();		//so this app. returns an underlying implem. of iostreams
		vertexShaderCode = vertexCodeStream.str();	//  which '<<' is overloaded to fully copy, for some reason
		vertexShaderSource = vertexShaderCode.c_str();	//ERROR: Make sure vertexShaderCode stays until the shader is compiled,
	} catch(std::ifstream::failure e) {			//  else the c_str pointer is garbage
		std::cout << "ERROR::SHADER::VERTEX::FILE NOT READ" << std::endl;
	}

	std::string fragmentShaderCode;
	try {
		std::ifstream input(fragment_path);
		input.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		std::stringstream fragmentCodeStream;
		fragmentCodeStream << input.rdbuf();
		fragmentShaderCode = fragmentCodeStream.str();
		fragmentShaderSource = fragmentShaderCode.c_str();
	} catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FRAGMENT::FILE NOT READ" << std::endl;
	}
	std::string geometryShaderCode;
	if(geometry_path != "NO_PATH") {
		try {
			std::ifstream input(geometry_path);
			input.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			std::stringstream geometryCodeStream;
			geometryCodeStream << input.rdbuf();
			geometryShaderCode = geometryCodeStream.str();
			geometryShaderSource = geometryShaderCode.c_str();
		} catch(std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::GEOMETRY::FILE NOT READ" << std::endl;
		}
	}

	GLchar infoLog[512];
	GLint success;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
	}

	GLuint geometryShader;
	if(geometry_path != "NO_PATH") {
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
		glCompileShader(geometryShader);
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
		}
	}

	shaderProgram = glCreateProgram();	//member
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if(geometry_path != "NO_PATH") {
		glAttachShader(shaderProgram, geometryShader);
	}
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM:LINKING FAILED\n" <<
			infoLog << "\n\nFrom files:\nV: " <<
			vertex_path << "\nF: " << fragment_path << "\nG: " << geometry_path <<
			std::endl;
		throw 4;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

const GLuint Shader::getProgram() {
	return shaderProgram;
}


void Shader::Use() {
	glUseProgram(shaderProgram);
}
