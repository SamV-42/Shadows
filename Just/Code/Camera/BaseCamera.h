#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class BaseCamera {
protected:
  bool mDirty = false;

  glm::mat4 mView;
  glm::mat4 mProjection;

  glm::vec3 mCameraPos;
  glm::vec3 mCameraFront;
  glm::vec3 mCameraUp;
public:

  BaseCamera(glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight);

  glm::mat4 getView() const;
  glm::mat4 getProj() const;

  glm::vec3 & getCameraPos();

  void setCameraPos(const glm::vec3 & _CameraPos);
  void setCameraFront(const glm::vec3 & _CameraFront);
  void setCameraUp(const glm::vec3 & _CameraUp);

  void updateView();
};

#endif
