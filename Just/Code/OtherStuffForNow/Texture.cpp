#include <SOIL/SOIL.h>
#include <iostream>
#include "Texture.h"

Texture::Texture(GLuint _id, std::string _type, std::string _path) {
  id = _id;
  type = _type;
  path = _path;
}

Texture Texture::createTexture(std::string directory, std::string name, std::string type) {// a static function
  std::string location = directory + "/" + name;    //yeah whatever
  return createTexture(location, type);
}

Texture Texture::createTexture(std::string location, std::string type) {  // a static function
  GLuint text;
  glGenTextures(1, &text);

  glBindTexture(GL_TEXTURE_2D, text);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height;
  unsigned char* image = SOIL_load_image(location.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
  if(!image) {
    std::cout << "ERROR:: failed to load texture from " << location << std::endl;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);

  glBindTexture(GL_TEXTURE_2D, 0);

  Texture result = Texture(text, type, location);
  return result;
}