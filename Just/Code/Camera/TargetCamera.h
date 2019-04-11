#ifndef TARGET_CAMERA_H
#define TARGET_CAMERA_H

#include "BaseCamera.h"

class TargetCamera : public BaseCamera {
private:
  glm::vec3 * mPlayerPosition;
public:
  TargetCamera(glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight, glm::vec3 * _PlayerPosition);
  void updatePlayerPosition();
};
#endif
