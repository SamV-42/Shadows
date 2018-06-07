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

Mesh* mesh; //dont even care anymore

GLFWwindow* window;
Shader* model_shader;
Shader* light_shader;
Shader* overlay_shader;

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

	model_shader = new Shader("shaders/main.vs", "shaders/main.fs");
	overlay_shader = new Shader("shaders/overlayer.vs", "shaders/overlayer.fs");
	light_shader = new Shader("shaders/lighting.vs", "shaders/lighting.fs");

	camera = new Camera(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT);

    {
        std::vector<Vertex> lertices; 
        {
        Vertex v1; v1.Position = glm::vec3(0.0f, 0.0f, 0.0f); v1.Normal = glm::vec3(0.0f, 0.0f, -1.0f); v1.TexCoords = glm::vec2(0.0f, 0.0f); 
        Vertex v2; v2.Position = glm::vec3(1.0f, 0.0f, 0.0f); v2.Normal = glm::vec3(0.0f, 0.0f, -1.0f); v2.TexCoords = glm::vec2(1.0f, 0.0f); 
        Vertex v3; v3.Position = glm::vec3(0.0f, 1.0f, 0.0f); v3.Normal = glm::vec3(0.0f, 0.0f, -1.0f); v3.TexCoords = glm::vec2(0.0f, 1.0f); 
        Vertex v4; v4.Position = glm::vec3(1.0f, 1.0f, 0.0f); v4.Normal = glm::vec3(0.0f, 0.0f, -1.0f); v4.TexCoords = glm::vec2(1.0f, 1.0f); 
        lertices.push_back(v1); lertices.push_back(v2); lertices.push_back(v3); lertices.push_back(v4);
        }
        std::vector<GLuint> indices; std::vector<Texture> textures;
        indices.push_back(0); indices.push_back(2); indices.push_back(1); indices.push_back(1); indices.push_back(2); indices.push_back(3);
        GLuint text = TextureFromFile("window.png","assets");
        Texture texty; texty.id = text; texty.type = "texture_diffuse"; texty.path="assets/window.png";
        textures.push_back(texty);

        mesh = new Mesh(lertices, indices, textures);
    }
    Model moodle = Model("assets/nano/nanosuit.obj");
    Model soodle = Model("assets/textures/hst.3ds");

    GLuint FBO; glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        GLuint outputText; glGenTextures(1, &outputText);
        glBindTexture(GL_TEXTURE_2D, outputText);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputText, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        GLuint RBO; glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR:: Framebuffer failed to initialize." << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


float quadVertices[] = {  
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};	
    GLuint frameVAO; glGenVertexArrays(1, &frameVAO);
    glBindVertexArray(frameVAO);
        GLuint frameVBO; glGenBuffers(1, &frameVBO);
        glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
        glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (GLvoid*)(2*sizeof(float)) );
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);


	double timer = glfwGetTime();
	double changeTime = 0;

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		changeTime = timer - glfwGetTime();
		timer = glfwGetTime();

		if(handleStuff()) break;

//@!
glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glEnable(GL_DEPTH_TEST);
		glClearColor(0.6f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// | GL_STENCIL_BUFFER_BIT);

        model_shader->Use();
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


        light_shader->Use();

		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));

        for(int i = 0; i < tmp_numPointLights; ++i) {
		    glm::mat4 zodel = glm::mat4(1.0f);
            zodel = glm::translate(zodel, tmp_pointLights[i]);
		    zodel = glm::scale(zodel, glm::vec3(0.0002f));
		    glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(zodel));
            soodle.Draw(light_shader);
        }
//*
glBindFramebuffer(GL_FRAMEBUFFER, 0);
glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);

overlay_shader->Use();
glBindVertexArray(frameVAO);
glDisable(GL_DEPTH_TEST);
//glActiveTexture(GL_TEXTURE10);
glBindTexture(GL_TEXTURE_2D, outputText);
//glUniform1i(glGetUniformLocation(overlay_shader->getProgram(), "imagine" ), 10);   //"material." + 
glDrawArrays(GL_TRIANGLES, 0, 6);
glBindVertexArray(0);//*/

        /*light_shader->Use();
        //glUniform3f(glGetUniformLocation(model_shader->getProgram(), "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));
    	glm::mat4 zodel = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(zodel));

        mesh->Draw(light_shader);*/

		glfwSwapBuffers(window);
	}

	delete model_shader;
	delete light_shader;

	glfwTerminate();

	return 0;
}



	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
