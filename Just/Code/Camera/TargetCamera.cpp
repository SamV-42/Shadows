#include "TargetCamera.h"

TargetCamera::TargetCamera(glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight, glm::vec3 * _PlayerPosition)
: BaseCamera(_cameraPos, _cameraFront, _cameraUp, kWidth, kHeight) {
  mPlayerPosition = _PlayerPosition;
}

void TargetCamera::updatePlayerPosition() {
  mCameraFront = glm::normalize(*mPlayerPosition - mCameraPos);
}
