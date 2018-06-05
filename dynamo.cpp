// Sources: this, Shader.cpp, utils.cpp, camera->cpp
// Libraries: GL, SOIL, GLEW, glfw
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/projection.hpp>

#include "Shader.h"
#include "Camera.h"

#include "Model.h"  //includes Mesh.h

const GLuint WIDTH = 1200;
const GLuint HEIGHT = 800;

GLFWwindow* window;
Shader* model_shader;
Shader* light_shader;
Shader* outline_shader;

Camera* camera;

const static int tmp_numPointLights = 1;
static glm::vec3 tmp_pointLights[] = {
		glm::vec3(  0, 0, 0  )/*,
		glm::vec3(  10, 7, 2  ),
		glm::vec3(  4, 3, -6  ),
		glm::vec3(  0, -2, 8  )//*/
	};

/* Input Stuff ---------------------------------- */

GLfloat lastX = 400.0f;
GLfloat lastY = 300.0f;
bool mouseIn = true;
bool keys[1024]; //WASD, respectively
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if(action == GLFW_PRESS) {
		keys[key] = true;
	} else if(action == GLFW_RELEASE) {
		keys[key] = false;
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if(mouseIn) {
		lastX = xpos;
		lastY = ypos;
		mouseIn = false;
	}
	GLfloat dx = xpos - lastX;
	GLfloat dy = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05f;
	dx *= sensitivity;
	dy *= sensitivity;

	camera->yaw += dx;
	camera->pitch -= dy;

	if(camera->pitch > camera->MAX_PITCH) camera->pitch = camera->MAX_PITCH;
	else if(camera->pitch < camera->MIN_PITCH) camera->pitch = camera->MIN_PITCH;

	camera->updateFront();
}

/* Input Handling -------------------------------------------------------------------*/
bool handleStuff() {
	glm::vec3 temp;
	glm::vec3 temp_2;
	glm::vec3 groundNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
		if(keys[GLFW_KEY_ESCAPE]) {
			return true;
		}

		switch(keys[GLFW_KEY_U] - keys[GLFW_KEY_J]) {
			case 1:
				tmp_pointLights[0] += camera->getMove(FORE, groundNormal); break;
			case -1:
				tmp_pointLights[0] += camera->getMove(BACK, groundNormal); break;
		}
		switch(keys[GLFW_KEY_H] - keys[GLFW_KEY_K]) {
			case 1:
				tmp_pointLights[0] += camera->getMove(LEFT, groundNormal); break;
			case -1:
				tmp_pointLights[0] += camera->getMove(RIGHT, groundNormal); break;
		}
		switch(keys[GLFW_KEY_L] - keys[GLFW_KEY_O]) {
			case 1:
				tmp_pointLights[0] += camera->getMove(DOWN, groundNormal); break;
			case -1:
				tmp_pointLights[0] += camera->getMove(UP, groundNormal); break;
		}

		switch(keys[GLFW_KEY_W] - keys[GLFW_KEY_S]) {
			case 1:
				camera->cameraPos += camera->getMove(FORE, groundNormal); break;
			case -1:
				camera->cameraPos += camera->getMove(BACK, groundNormal); break;
		}
		switch(keys[GLFW_KEY_A] - keys[GLFW_KEY_D]) {
			case 1:
				camera->cameraPos += camera->getMove(LEFT, groundNormal); break;
			case -1:
				camera->cameraPos += camera->getMove(RIGHT, groundNormal); break;
		}
		if(keys[GLFW_KEY_SPACE]) {
			if(keys[GLFW_KEY_LEFT_SHIFT] or keys[GLFW_KEY_RIGHT_SHIFT]) {
				camera->cameraPos += camera->getMove(DOWN, groundNormal);
			} else {
				camera->cameraPos += camera->getMove(UP, groundNormal);
			}
		}
		//camera->cameraFront = glm::normalize(movingStuff.back().position - camera->cameraPos);
		camera->updateView();
		return false;
}
/* Utility ------------------------------------------------------------------------- */

void initGL() {
	if(!glfwInit()) { std::cout << "GLFW Init error!" << std::endl; throw 1; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
	if(!window) { std::cout << "Create Window error!" << std::endl; throw 2; }
	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;	//apparently needs current window before init
	GLenum err = glewInit();
	if(err) { std::cout << "GLEW Init Error!: " << glewGetErrorString(err) << std::endl; throw 3; }

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0,0,width,height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
}

// Display functions -------------------------------------------------------------------------------- 
void sendPointLights(Shader* default_shader) {
	std::string tempPlace = "pointLights[X].";
	for(int i = 0; i < tmp_numPointLights; ++i) {	//int i = 0; i < light.translations.size(); ++i
		tempPlace[12] = (char)('0' + i);
		GLint lightPosLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "position").c_str() );
		glUniform3f(lightPosLoc, tmp_pointLights[i].x, tmp_pointLights[i].y, tmp_pointLights[i].z);
		GLint lightConstantLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "constant").c_str() );
		glUniform1f(lightConstantLoc, 1.0f);
		GLint lightLinearLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "linear").c_str() );
		glUniform1f(lightLinearLoc, 0.05f);
		GLint lightQuadraticLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "quadratic").c_str() );
		glUniform1f(lightQuadraticLoc, 0.01f);

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	GLint lightAmbientLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "ambient").c_str() );
	GLint lightDiffuseLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "diffuse").c_str() );
	GLint lightSpecularLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "specular").c_str() );
	glm::vec3 lightAmbient = lightColor * glm::vec3(0.05f);
	glUniform3f(lightAmbientLoc, lightAmbient.x, lightAmbient.y, lightAmbient.z);
	glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
	glUniform3f(lightDiffuseLoc, lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
	glUniform3f(lightSpecularLoc, lightColor.x, lightColor.y, lightColor.z);

	}
}

/* Main ----------------------------------------------------------------------------------*/
int main() {

	initGL();

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	model_shader = new Shader("main.vs", "main.fs");
	outline_shader = new Shader("outliner.vs", "outliner.fs");
	light_shader = new Shader("lighting.vs", "lighting.fs");

	camera = new Camera(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT);

    Model moodle = Model("nano/nanosuit.obj");
    Model soodle = Model("textures/hst.3ds");

	double timer = glfwGetTime();
	double changeTime = 0;

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		changeTime = timer - glfwGetTime();
		timer = glfwGetTime();

		if(handleStuff()) break;

		glClearColor(0.6f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        model_shader->Use();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glUniform3f(glGetUniformLocation(model_shader->getProgram(), "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));
        glUniform1f(glGetUniformLocation(model_shader->getProgram(), "material0.shininess"), 64);
		glm::mat4 podel = glm::mat4(1.0f);
        podel = glm::translate(podel, glm::vec3(0.0f));
		podel = glm::scale(podel, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(podel));
        sendPointLights(model_shader);
        moodle.Draw(model_shader);

    outline_shader->Use();
    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
		glUniform3f(glGetUniformLocation(outline_shader->getProgram(), "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(outline_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(outline_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));
		//podel = glm::scale(podel, glm::vec3(1.2f));
		glUniformMatrix4fv(glGetUniformLocation(outline_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(podel));
        moodle.Draw(outline_shader);

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF);    //didn't think I needed this... but glClear is affected by it geez
/*

        light_shader->Use();

		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));

        for(int i = 0; i < tmp_numPointLights; ++i) {
		    glm::mat4 zodel = glm::mat4(1.0f);
            zodel = glm::translate(zodel, tmp_pointLights[i]);
		    zodel = glm::scale(zodel, glm::vec3(0.0005f));
		    glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(zodel));
            soodle.Draw(light_shader);
        }*/

		glfwSwapBuffers(window);
	}

	delete model_shader;
	delete light_shader;

	glfwTerminate();

	return 0;
}



	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
