#ifndef FOLLOW_CAMERA_H
#define FOLLOW_CAMERA_H

#include "BaseCamera.h"

class FollowCamera : public BaseCamera {
private:
  glm::vec3 * mPlayerPosition;
  double kMinDistance;
  double kMaxDistance;
  double kDistAccel;
  double kAngleAccel;
public:
  FollowCamera(glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight, glm::vec3 * _PlayerPosition);
  void updatePlayerPosition();
};
#endif
