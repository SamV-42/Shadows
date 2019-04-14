#include "PlayerView.h"
#include "ModelWrapper.h"

#include <iostream>

ModelWrapper::ModelWrapper(std::string path, int shaderIndex) : mModel(path), mShaderIndex(shaderIndex) { }

glm::mat4 & ModelWrapper::getModelMtx() {
  return mModelMtx;
}

void ModelWrapper::setModelMtx(const glm::mat4 & modelMtx) {
  mModelMtx = modelMtx;
}

void ModelWrapper::translate(glm::vec3 translation) {
  mModelMtx = glm::translate(mModelMtx, translation);
}

void ModelWrapper::draw(Shader* shader) {
  mModel.Draw(shader);
}

glm::vec3 ModelWrapper::getTranslation() {
  return glm::vec3(mModelMtx[3]); //glm uses column-major order
}
