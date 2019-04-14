#include "TargetCamera.h"

TargetCamera::TargetCamera(glm::vec3 _cameraPos, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight, glm::vec3 _PlayerPosition)
: BaseCamera(_cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), _cameraUp, kWidth, kHeight) {
  mPlayerPosition = _PlayerPosition;
}

void TargetCamera::updatePlayerPosition() {
  mDirty = true;
  mCameraFront = glm::normalize(mPlayerPosition - mCameraPos);
}
