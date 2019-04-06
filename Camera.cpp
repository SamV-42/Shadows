#include "Camera.h"	//also has the other headers -- prob not good form
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 proj(glm::vec3 thing, glm::vec3 normal) {
    return thing * (GLfloat)( glm::dot(normal, thing) / glm::dot(normal, normal) );
}


void Camera::updateFront() {
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

Camera::Camera(glm::vec3 cameraPos_, glm::vec3 cameraFront_, glm::vec3 cameraUp_, GLuint WIDTH, GLuint HEIGHT) {
	cameraPos = cameraPos_;
	cameraFront = cameraFront_;
	cameraUp = cameraUp_;

	projection = glm::perspective(glm::radians(60.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    updateFront();
    updateView();
}

glm::vec3 Camera::getMove(CamDir dir, glm::vec3 groundNormal = glm::vec3(0.0f, 1.0f, 0.0f)) {
	switch(dir) {
		case LEFT: { glm::vec3 temp = glm::cross(cameraFront, cameraUp);
			return glm::normalize(temp - proj(temp, groundNormal)) * -cameraSpeed; };
		case RIGHT: { glm::vec3 temp = glm::cross(cameraFront, cameraUp);
			return glm::normalize(temp - proj(temp, groundNormal)) * cameraSpeed; };
		case FORE: return cameraSpeed * glm::normalize(cameraFront - proj(cameraFront, groundNormal));
		case BACK: return -cameraSpeed * glm::normalize(cameraFront - proj(cameraFront, groundNormal));
		case UP: return cameraSpeed * groundNormal;
		case DOWN: return -cameraSpeed * groundNormal;
	};
    return glm::vec3(0,0,0);
}

void Camera::updateView() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
