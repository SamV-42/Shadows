#include "ModelWrapper.h"

inline glm::mat4 ModelWrapper::getModelMtx() const {
  return mModelMtx;
}

void ModelWrapper::setModelMtx(const glm::mat4 & modelMtx) {
  mModelMtx = modelMtx;
}

void ModelWrapper::translate(glm::vec3 translation) {
  mModelMtx = glm::translate(mModelMtx, translation);
}
