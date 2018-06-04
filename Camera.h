#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum CamDir { LEFT, RIGHT, FORE, BACK, UP, DOWN };

class Camera {
public:
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	GLfloat yaw = 0.0f;	//hooray c++ 11 and member initializing in headers
	GLfloat pitch = 0.0f;
	GLfloat cameraSpeed = 0.05f;

	GLfloat MIN_PITCH = -89.0f;
	GLfloat MAX_PITCH = 89.0f;

	Camera(glm::vec3 cameraPos_, glm::vec3 cameraFront_, glm::vec3 cameraUp_, GLuint WIDTH, GLuint HEIGHT);

	void updateFront();
	void updateView();
	glm::vec3 getMove(CamDir dir, glm::vec3 groundNormal);
};

#endif
