/* Note: include after other stuff. GLFW requires GLEW come before it the way I'm using it. GLFW just added here just in case, shouldn't be needed. */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cctype>
#include <utility>
#include <cstddef>
#include <type_traits>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#define SHADERLIST_UBO Matrices, FragmentData, PointLights
enum ShaderListUBOEnum { SHADERLIST_UBO };

#define SHADERLIST material0, model, view, projection, transform
enum ShaderListEnum { SHADERLIST };
//Outside classes can tell it, eg, shader.send3f(ShaderListEnum.viewPos, 1.0f,2.0f,3.0f);
//  Define all uniforms across all shaders here -- getUniform returns -1 on 'not found', which is fine
//  calls the appropriate glUniform function, which on -1 silently returns




class Shader {
public:

	static void initUBOBuffers();	//called in PlayerView.initBufferData
	static void initUBO(ShaderListUBOEnum ubo, std::size_t size);
	template <typename T> static void loadUBO(ShaderListUBOEnum ubo, const T & value, size_t offset);


	//static void loadUBO(ShaderListUBOEnum ubo, const GLint & value, size_t offset);
	//static void loadUBO(ShaderListUBOEnum ubo, const GLfloat & value, size_t offset);


	//defined at the bottom of this header, because templates are apparently dumb

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
	static std::vector<GLuint> sUBOLocations;
	void loadUniformLocations();	//call AT THE END of the constructor! aka after shader is made!


	//-------------------------------------------------------------------------------------------
	//The below code is to enable the aforementioned setValue function
	//Stupid macro stuff below WARNING!
	//-------------------------------------------------------------------------------------------

	//stores in: 	std::vector<std::string> ShaderListString;
	#define __SHADERLISTSTRINGS(x...) _SHADERLISTSTRINGS(x)
	#define _SHADERLISTSTRINGS(x...) #x
	#define SHADERLISTSTRINGS(x...) \
		{ \
			std::stringstream splitline(__SHADERLISTSTRINGS(x)); \
			std::string split; \
			while(std::getline(splitline, split, ',')) { \
				while(std::isspace(split.front())) { \
					split.erase(split.begin()); \
				} \
				while(std::isspace(split.back())) { \
					split.erase(split.end()); \
				} \
				ShaderLists.at(i).push_back(split); \
			} \
			i++; \
		}

public:

private:

	class StoopidSubClass {
	public:
		std::vector<std::vector<std::string> > ShaderLists;
		StoopidSubClass() {
			ShaderLists.push_back(std::vector<std::string>());
			ShaderLists.push_back(std::vector<std::string>());
			int i = 0;
			SHADERLISTSTRINGS( SHADERLIST ) ;
			SHADERLISTSTRINGS( SHADERLIST_UBO ) ;
		}
	};

	std::vector<std::vector<std::string> > & _stupidMacroSetupFunction() {
		static StoopidSubClass shaderListMaker;
		return shaderListMaker.ShaderLists;
	}
};

template <typename T>
void Shader::loadUBO(ShaderListUBOEnum ubo, const T & value, std::size_t offset) {
	std::cout << "T: " << sizeof(T) << std::endl;
	glBindBuffer(GL_UNIFORM_BUFFER, sUBOLocations[ubo]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), glm::value_ptr(value));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <> void Shader::loadUBO<GLint>(ShaderListUBOEnum ubo, const GLint & value, size_t offset);
template <> void Shader::loadUBO<GLfloat>(ShaderListUBOEnum ubo, const GLfloat & value, size_t offset);

/*template <>
void Shader::loadUBO<float>(ShaderListUBOEnum ubo, const float & value, std::size_t offset) {
	glBindBuffer(GL_UNIFORM_BUFFER, sUBOLocations[ubo]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &value);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}*/

#endif
