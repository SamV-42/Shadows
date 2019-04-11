#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

#include <GLFW/glfw3.h>

class Shader {
public:
  Shader(std::string vertex_path, std::string fragment_path);

  const GLuint getProgram();
  void use();

private:
  GLuint shaderProgram;
};

#endif
