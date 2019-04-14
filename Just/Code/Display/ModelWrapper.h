#ifndef MODELWRAPPER_H
#define MODELWRAPPER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"

class ModelWrapper {
public:
  ModelWrapper(std::string path, int shaderIndex);

  Model mModel;

  void draw(Shader* shader);

  glm::mat4 & getModelMtx();
  void setModelMtx(const glm::mat4 & modelMtx);
  void translate(glm::vec3 translation);
  glm::vec3 getTranslation();

private:
  int mShaderIndex;
  glm::mat4 mModelMtx;
};

#endif
