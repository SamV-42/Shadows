#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

Shader::Shader(std::string vertex_path, std::string fragment_path) {
  const GLchar* vertexShaderSource;   //pointers to the starts of c strings
  const GLchar* fragmentShaderSource;

  std::string vertexShaderCode; //define this out of the try block so the pointer to it doesn't garbify
  try {
    std::ifstream input(vertex_path); //implicit construction of input -- C++11 has a string constructor
    input.exceptions (std::ifstream::failbit | std::ifstream::badbit);  //when the ifstream will throw
    std::stringstream vertexCodeStream; //rdbuf returns the basic_ifstream object;
    vertexCodeStream << input.rdbuf();  // << overloaded for stringstream (ostream) to output contents of stream char by char
    vertexShaderCode = vertexCodeStream.str();  //convert our ostream into a string
    vertexShaderSource = vertexShaderCode.c_str();
  } catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER VERTEX: File not found" << std::endl;
  }

  std::string fragmentShaderCode; //define this out of the try block so the pointer to it doesn't garbify
  try {
    std::ifstream input(fragment_path); //implicit construction of input -- C++11 has a string constructor
    input.exceptions (std::ifstream::failbit | std::ifstream::badbit);  //when the ifstream will throw
    std::stringstream fragmentCodeStream; //rdbuf returns the basic_ifstream object;
    fragmentCodeStream << input.rdbuf();  // << overloaded for stringstream (ostream) to output contents of stream char by char
    fragmentShaderCode = fragmentCodeStream.str();  //convert our ostream into a string
    fragmentShaderSource = fragmentShaderCode.c_str();
  } catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER FRAGMENT: File not found" << std::endl;
  }

  GLchar infoLog[512];
  GLint success;

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //NULL -- strings assumed to be null-term
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);   //length not needed -NULL
    //cout << overloaded to output cstrings with char* ptr
    std::cout << "ERROR::SHADER VECTOR: Compiling failed with error> " << infoLog << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //NULL -- strings assumed to be null-term
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);   //length not needed -NULL
    //cout << overloaded to output cstrings with char* ptr
    std::cout << "ERROR::SHADER FRAGMENT: Compiling failed with error> " << infoLog << std::endl;
  }

  shaderProgram = glCreateProgram();  //member
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER PROGRAM: Linking failed with error> " << infoLog << std::endl;
    throw 4;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

}

const GLuint Shader::getProgram() {
  return shaderProgram;
}

void Shader::use() {
  glUseProgram(shaderProgram);
}
