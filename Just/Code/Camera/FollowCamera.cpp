#include "FollowCamera.h"

FollowCamera::FollowCamera(glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, GLuint kWidth, GLuint kHeight, glm::vec3 * _PlayerPosition)
 : BaseCamera(_cameraPos, _cameraFront, _cameraUp, kWidth, kHeight) {
  mPlayerPosition = _PlayerPosition;
}

void FollowCamera::updatePlayerPosition() {
  //won't bother with mDirty checking, just pray the compiler takes it out or that it doesn't matter

  /*
  Do this later
  */

  /*

  //Do we need a previous_position type deal?
  //Could we represent player movement with PropsedMove or something?

  mCameraPos =
  mCameraUp =
  mCameraFront =
  */


  mDirty = true;
}
