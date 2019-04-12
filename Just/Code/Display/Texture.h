#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture {
private:
  Texture(GLuint _id, std::string _type, std::string _path);
public:
  static Texture createTexture(std::string name, std::string directory, std::string type);  //prob broken
  static Texture createTexture(std::string location, std::string type);

  GLuint id;
  std::string type;
  std::string path;

};

#endif
