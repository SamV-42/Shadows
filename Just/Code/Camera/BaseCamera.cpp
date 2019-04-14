#include "BaseCamera.h"	//also has the other headers -- prob not good form
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "debug.h"

BaseCamera::BaseCamera(glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight) {
	mCameraPos = _cameraPos;
	mCameraFront = _cameraPos;
	mCameraUp = _cameraUp;

	mProjection = glm::perspective(glm::radians(45.0f), (float)kWidth / (float)kHeight, 0.1f, 100.0f);
}

void BaseCamera::updateView() {
  if(mDirty) {
    mView = glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
    mDirty = false;
  }
}

glm::mat4 BaseCamera::getView() const {
  assert(!mDirty);
  return mView;
}

glm::mat4 BaseCamera::getProj() const {
  assert(!mDirty);
  return mProjection;
}

void BaseCamera::setCameraPos(const glm::vec3 & _CameraPos) {
  if(mCameraPos != _CameraPos) {
    mCameraPos = _CameraPos;
    mDirty = true;
  }
}

void BaseCamera::setCameraFront(const glm::vec3 & _CameraFront) {
  if(mCameraFront != _CameraFront) {
    mCameraFront = _CameraFront;
    mDirty = true;
  }
}

void BaseCamera::setCameraUp(const glm::vec3 & _CameraUp) {
  if(mCameraUp != _CameraUp) {
    mCameraUp = _CameraUp;
    mDirty = true;
  }
}

glm::vec3 & BaseCamera::getCameraPos() {
	return mCameraPos;
}
