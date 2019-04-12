#ifndef MODELWRAPPER_H
#define MODELWRAPPER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.h"

class ModelWrapper {
public:
  ModelWrapper(std::string path) : mModel(path) {};

  Model mModel;

  inline glm::mat4 getModelMtx() const;
  void setModelMtx(const glm::mat4 & modelMtx);
  void translate(glm::vec3 translation);

private:
  glm::mat4 mModelMtx;
};

#endif
