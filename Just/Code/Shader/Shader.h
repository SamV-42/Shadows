/* Note: include after other stuff. GLFW requires GLEW come before it the way I'm using it. GLFW just added here just in case, shouldn't be needed. */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#define SHADERLIST viewPos, material0, model, view, projection, transform
//Outside classes can tell it, eg, shader.send3f(ShaderListEnum.viewPos, 1.0f,2.0f,3.0f);
//  Define all uniforms across all shaders here -- getUniform returns -1 on 'not found', which is fine
//  calls the appropriate glUniform function, which on -1 silently returns
enum ShaderListEnum { SHADERLIST};

class Shader {
public:
	Shader(std::string vertex_path, std::string fragment_path);
	Shader(std::string vertex_path, std::string fragment_path, std::string geometry_path);

	const GLuint getProgram();
	void use();

	void send1f(ShaderListEnum uniform, GLfloat input1);
	void send2f(ShaderListEnum uniform, GLfloat input1, GLfloat input2);
	void send3f(ShaderListEnum uniform, GLfloat input1, GLfloat input2, GLfloat input3);
	void send4f(ShaderListEnum uniform, GLfloat input1, GLfloat input2, GLfloat input3, GLfloat input4);
	void send1i(ShaderListEnum uniform, GLint input1);
	void send2i(ShaderListEnum uniform, GLint input1, GLint input2);
	void send3i(ShaderListEnum uniform, GLint input1, GLint input2, GLint input3);
	void send4i(ShaderListEnum uniform, GLint input1, GLint input2, GLint input3, GLint input4);
	void sendMatrix2fv(ShaderListEnum uniform, const glm::mat2 & inputMtx);
	void sendMatrix3fv(ShaderListEnum uniform, const glm::mat3 & inputMtx);
	void sendMatrix4fv(ShaderListEnum uniform, const glm::mat4 & inputMtx);

public:
	GLuint shaderProgram;

	std::vector<GLint> mUniformLocations;
	void loadUniformLocations();	//call AT THE END of the constructor! aka after shader is made!




	//-------------------------------------------------------------------------------------------
	//The below code is to enable the aforementioned setValue function
	//Stupid macro stuff below WARNING!
	//-------------------------------------------------------------------------------------------

	//stores in: 	std::vector<std::string> ShaderListString;
	#define __SHADERLISTSTRINGS(x...) _SHADERLISTSTRINGS(x)
	#define _SHADERLISTSTRINGS(x...) #x
	#define SHADERLISTSTRINGS \
		{ \
			std::stringstream splitline(__SHADERLISTSTRINGS(SHADERLIST)); \
			std::string split; \
			while(std::getline(splitline, split, ',')) { \
				while(std::isspace(split.front())) { \
					split.erase(split.begin()); \
				} \
				while(std::isspace(split.back())) { \
					split.erase(split.end()); \
				} \
				ShaderListString.push_back(split); \
			} \
		}

	public:

	private:

		class StoopidSubClass {
		public:
			std::vector<std::string> ShaderListString;
			StoopidSubClass() {
				SHADERLISTSTRINGS //( SHADERLIST ) ;
			}
		};

		std::vector<std::string> & _stupidMacroSetupFunction() {
			static StoopidSubClass ShaderListString;
			return  ShaderListString.ShaderListString;
		}
};

#endif
