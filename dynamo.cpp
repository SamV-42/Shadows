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

glm::vec3 position = glm::vec3(0.0f, -1.25f, 0.0f);
GLfloat direction = glm::radians(0.0f);

GLFWwindow* window;
Shader* model_shader;
Shader* light_shader;
Shader* outline_shader;

Camera* camera;

const static int tmp_numPointLights = 4;
static glm::vec3 tmp_pointLights[] = {
		glm::vec3(  5, 0, 7  ),
		glm::vec3(  2, 0, 3  ),
		glm::vec3(  4, 0, -6  ),
		glm::vec3(  0, 0, 8  )//*/
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
/*void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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
}*/

/* Input Handling -------------------------------------------------------------------*/

bool handleStuff() {
	glm::vec3 groundNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
		if(keys[GLFW_KEY_ESCAPE]) {
			return true;
		}

        //glm::vec3 direction = position - camera->cameraPos;
        //position += ( (GLfloat)(keys[GLFW_KEY_W] - keys[GLFW_KEY_S])*( 0.075f * glm::normalize(glm::vec3(direction.x, 0, direction.z)) ) )  +  ( (GLfloat)(keys[GLFW_KEY_D] - keys[GLFW_KEY_A])*( 0.075f*glm::normalize(glm::cross(glm::vec3(camera->cameraFront.x, 0.0f, camera->cameraFront.z), groundNormal)) ) );
        direction += glm::radians(180*(GLfloat)(keys[GLFW_KEY_A] - keys[GLFW_KEY_D]));

        //std::cout << direction << std::endl;

        GLfloat scale_fact = 0.075f*(GLfloat)(keys[GLFW_KEY_W] - keys[GLFW_KEY_S]);
        glm::vec4 change = glm::rotate(glm::mat4(1.0f), glm::radians(direction), glm::vec3(0.0f, 1.0f, 0.0f))*glm::vec4(0.0f, 0.0f, scale_fact, 1.0f);
        position.x += change.x; position.z += change.z;

		camera->cameraFront = glm::normalize(position - camera->cameraPos);
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
	//glfwSetCursorPosCallback(window, mouse_callback);
}

// Display functions -------------------------------------------------------------------------------- 
void sendPointLights(Shader* default_shader, GLfloat atn_c = 1.0f, GLfloat atn_l = 0.05f, GLfloat atn_q = 0.001f) {
	std::string tempPlace = "pointLights[X].";
	for(int i = 0; i < tmp_numPointLights; ++i) {	//int i = 0; i < light.translations.size(); ++i
		tempPlace[12] = (char)('0' + i);
		GLint lightPosLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "position").c_str() );
		glUniform3f(lightPosLoc, tmp_pointLights[i].x, tmp_pointLights[i].y, tmp_pointLights[i].z);
		GLint lightConstantLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "constant").c_str() );
		glUniform1f(lightConstantLoc, atn_c);
		GLint lightLinearLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "linear").c_str() );
		glUniform1f(lightLinearLoc, atn_l);
		GLint lightQuadraticLoc = glGetUniformLocation(default_shader->getProgram(), (tempPlace + "quadratic").c_str() );
		glUniform1f(lightQuadraticLoc, atn_q);

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
	outline_shader = new Shader("shaders/outliner.vs", "shaders/outliner.fs");
	light_shader = new Shader("shaders/lighting.vs", "shaders/lighting.fs");

	camera = new Camera(glm::vec3(0.0f, 2.8f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT);

    Mesh* mesh;
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
        indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(1); indices.push_back(3); indices.push_back(2);
        GLuint text = TextureFromFile("window.png","assets");
        GLuint text2 = TextureFromFile("window_specular.png","assets");
        Texture texty; texty.id = text; texty.type = "texture_diffuse"; texty.path="assets/window.png";
        Texture texty2; texty2.id = text; texty2.type = "texture_specular"; texty2.path="assets/window_specular.png";
        textures.push_back(texty);
        textures.push_back(texty2);
        mesh = new Mesh(lertices, indices, textures);
    }


    Model moodle = Model("assets/nano/nanosuit.obj");
    Model soodle = Model("assets/textures/hst.3ds");

	double timer = glfwGetTime();
	double changeTime = 0;

    glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		changeTime = timer - glfwGetTime();
		timer = glfwGetTime();

		if(handleStuff()) break;

		glClearColor(0.6f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        model_shader->Use();

		glUniform3f(glGetUniformLocation(model_shader->getProgram(), "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));
        glUniform1f(glGetUniformLocation(model_shader->getProgram(), "material0.shininess"), 64);
		glm::mat4 podel = glm::mat4(1.0f);
        podel = glm::translate(podel, position);
		podel = glm::scale(podel, glm::vec3(0.12f));
        podel = glm::rotate(podel, glm::radians(direction), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(podel));
        sendPointLights(model_shader, 1.0f, 0.02, 0.003);
        moodle.Draw(model_shader);


        light_shader->Use();

		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));

        for(int i = 0; i < tmp_numPointLights; ++i) {
		    glm::mat4 zodel = glm::mat4(1.0f);
            zodel = glm::translate(zodel, tmp_pointLights[i]);
		    zodel = glm::scale(zodel, glm::vec3(0.0005f, 0.004, 0.0005f));
		    glUniformMatrix4fv(glGetUniformLocation(light_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(zodel));
            soodle.Draw(light_shader);
        }


        model_shader->Use();
        glUniform3f(glGetUniformLocation(model_shader->getProgram(), "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(camera->projection));
        glUniform1f(glGetUniformLocation(model_shader->getProgram(), "material0.shininess"), 64);
        sendPointLights(model_shader, 1.0f, 1.0f, 0.002f);
    	glm::mat4 zodel = glm::mat4(1.0f);
        zodel = glm::translate(zodel, glm::vec3(-5.0f, -1.25f, 0.0f));
        zodel = glm::rotate(zodel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        zodel = glm::scale(zodel, glm::vec3(12.0f));
        glUniformMatrix4fv(glGetUniformLocation(model_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(zodel));

        mesh->Draw(model_shader);

		glfwSwapBuffers(window);
	}

	delete model_shader;
	delete light_shader;

	glfwTerminate();

	return 0;
}



	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
