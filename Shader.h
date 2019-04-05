/* Note: include after other stuff. GLFW requires GLEW come before it the way I'm using it. GLFW just added here just in case, shouldn't be needed. */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GLFW/glfw3.h>
#include <vector>

class Shader {
public:
	Shader(std::string vertex_path, std::string fragment_path);
	Shader(std::string vertex_path, std::string fragment_path, std::string geometry_path);

	const GLuint getProgram();
	void Use();

private:
	GLuint shaderProgram;

};

#endif
