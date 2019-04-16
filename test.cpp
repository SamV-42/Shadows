#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>

GLuint target = glGetUniformBlockIndex(mShaders[0].getProgram(), "PointLights");
std::cout << "Index: " << target << std::endl;
GLint number;
glGetActiveUniformBlockiv(mShaders[0].getProgram(), target, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &number);
std::cout << "Number: " << number << std::endl;
GLint* params = new GLint[number];
glGetActiveUniformBlockiv(mShaders[0].getProgram(), target, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, params);
//glGetActiveUniformBlockiv(mShaders[0].getProgram(), target, GL_UNIFORM_BLOCK_DATA_SIZE, &params);
//  std::cout << "#: " << params << std::endl;
for(int i = 0; i < number; ++i) {
	GLint type, offset;
	const GLuint &index = (GLuint)params[i];
	GLchar neame[256];
	glGetActiveUniformName(mShaders[0].getProgram(), index, 256, 0, neame);
	glGetActiveUniformsiv(mShaders[0].getProgram(), 1, &index, GL_UNIFORM_TYPE, &type);
	glGetActiveUniformsiv(mShaders[0].getProgram(), 1, &index, GL_UNIFORM_OFFSET, &offset);
	std::cout << "Name: " << neame << "\tType: " << type << "\tOffset: " << offset << std::endl;
}

class Stoopid {
public:
	template <typename T>
	static void testing(T value);
};

template <typename T>
void Stoopid::testing(T value) {
 std::cout << "T!:" << value << std::endl;
}

template <>
void Stoopid::testing<int>(int value) {
	std::cout << "int" << value << std::endl;
}

template <>
void Stoopid::testing<double>(double value) {
	std::cout << "dou" << value << std::endl;
}

int main() {
	Stoopid::testing(1);
	Stoopid::testing(1.5);
	Stoopid::testing(std::string("Hi"));
	Stoopid::testing('a');
}
