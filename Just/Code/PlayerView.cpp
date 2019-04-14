#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PlayerView.h"
#include "Logging.hpp"
#include "Simulation.h"
#include "Architecture.h"
#include "Display/ModelWrapper.h"

const GLuint WIDTH = 1200;
const GLuint HEIGHT = 800;


void PlayerView::initialize() {
  //these should all probably go through Architecture somehow
  //  since they touch the actual hardware, at the very least
   //  through file I/O if not also through openGL in general

  initInitializeOpenGL();
  initLoadShaders();
  initLoadModels();
  initBufferData();      //a few uniforms/UBOs, Camera
  initInputOutput();      //
  //note that for now, we're just going to try to keep everything in memory
  //so no more streaming in new significant data, it's all in the heap or buffers somewhere
}

bool PlayerView::shouldClose() {
  return glfwWindowShouldClose(mWindow) || shouldCloseVariable;
}

void PlayerView::respondToEvents() {
  glfwPollEvents();

  shouldCloseVariable = keys[GLFW_KEY_ESCAPE];

  double fb = keys[GLFW_KEY_W] - keys[GLFW_KEY_S];
  double rl = keys[GLFW_KEY_D] - keys[GLFW_KEY_A];

  double speed = (1 - mSlow * 0.5) * (fb == rl ? abs(fb) : 1);
  double angle = atan2(rl, fb);  //reversed so forward is 0, clockwise is positive

  Simulation::getInstance() .mInput1.percentForward = speed;
  Simulation::getInstance() .mInput1.angle = angle;
}
///*
const static int tmp_numPointLights = 4;
static glm::vec3 tmp_pointLights[] = {
		glm::vec3(  5, 0, 7  ),
		glm::vec3(  2, 0, 3  ),
		glm::vec3(  4, 0, -6  ),
		glm::vec3(  0, 0, 8  )
	};
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
}//*/

//BEACON
void PlayerView::updateView() {
  glClearColor(0.4f, 0.1f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  for(int i = 0; i < mModels.size(); ++i) {
    Shader & shader = mShaders.at(i);
    shader.use();

    shader.send3f(ShaderListEnum::viewPos, camera->getCameraPos().x, camera->getCameraPos().y, camera->getCameraPos().z);
    shader.sendMatrix4fv(ShaderListEnum::view, camera->getView());

    shader.sendMatrix4fv(ShaderListEnum::projection, camera->getProj());
    sendPointLights(&shader, 1.0f, 0.02, 0.003);
    glUniform1f(glGetUniformLocation(shader.getProgram(), "material0.shininess"), 64);

    for(auto& model : mModels.at(i)) {
      shader.sendMatrix4fv(ShaderListEnum::model, model.getModelMtx());

      model.draw(&shader);
    }
  }
  glfwSwapBuffers(mWindow);
}

void PlayerView::shutdown() {
  glfwTerminate();
}

PlayerView::PlayerView() { }

void PlayerView::initInitializeOpenGL() {

  if(!glfwInit()) { ERROR_LOG << "GLFW Init error!" << std::endl; throw 1; }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
  if(!mWindow) { ERROR_LOG << "Create Window error!" << std::endl; throw 2; }
  glfwMakeContextCurrent(mWindow);

  glewExperimental = GL_TRUE;	//apparently needs current window before init
  GLenum err = glewInit();
  if(err) { OUTPUT_LOG << "GLEW Init Error!: " << glewGetErrorString(err) << std::endl; throw 3; }

  int width, height;
  glfwGetFramebufferSize(mWindow, &width, &height);
  glViewport(0,0,width,height);

  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //note: callback functions here need to be static, so they'll call getInstance()
  glfwSetKeyCallback(mWindow, clbkKey);
  glfwSetCursorPosCallback(mWindow, clbkMouse);
}

void PlayerView::initLoadShaders() {
  auto shaderFile = Architecture::getInstance() .readFile("GameData/ShaderList.txt");
  const std::string shader_location("Code/Shader/Shaders/");
  for(auto& line : shaderFile) {
    std::vector<std::string> values = Architecture::getInstance() .splitLine(line);
    values[0] = shader_location + values[0];
    values[1] = shader_location + values[1];
    if(values[2] != "NO_PATH") {
      values[2] = shader_location + values[2];
    }
    mShaders.push_back(Shader(values[0], values[1], values[2]));
    mModels.push_back(std::vector<ModelWrapper>());
  }

  std::vector<std::string> ubo_lists = {"Matrices", "viewPos", "pointLights"};  //C++11 only!
  for(int i = 0; i < ubo_lists.size(); ++i) {
    for(auto& shader : mShaders) {
      GLuint index = glGetUniformBlockIndex(shader.getProgram(), ubo_lists[i].c_str() );
      glUniformBlockBinding(shader.getProgram(), index, i);
    }
  }
}

void PlayerView::initLoadModels() {
  auto modelsFile = Architecture::getInstance() .readFile("GameData/ModelList.txt");
  const std::string models_location("Assets/");
  int firstIndex = 0;
  for(auto& line : modelsFile) {
    std::vector<std::string> values = Architecture::getInstance() .splitLine(line);

    glm::mat4 initialModelMtx = glm::mat4(1.0f);
    initialModelMtx = glm::scale(initialModelMtx, glm::vec3(std::stof(values[7]), std::stof(values[8]), std::stof(values[9]) ));
    initialModelMtx = glm::rotate(initialModelMtx, glm::radians(std::stof(values[3])), glm::vec3(std::stof(values[4]), std::stof(values[5]), std::stof(values[6])));
    initialModelMtx = glm::translate(initialModelMtx, glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2])));

    int modelShaderIndex = std::stoi(values[10]);
    ModelWrapper model = ModelWrapper("Assets/" + values[11], modelShaderIndex);
    model.setModelMtx(initialModelMtx);
    mModels.at(modelShaderIndex).push_back(model);
    if(!firstIndex) {
      firstIndex = modelShaderIndex;
    }
  }
  mPlayer = &(mModels.at(firstIndex).at(0));
}

void PlayerView::initBufferData() {
  camera = new TargetCamera(glm::vec3(0.0f, 2.8f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT, mPlayer->getTranslation());
  camera->updatePlayerPosition();
  camera->updateView();
}

void PlayerView::initInputOutput() {
  //<<<>>>
}

void PlayerView::clbkKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if(action == GLFW_PRESS) {
    keys[key] = true;
  } else if(action == GLFW_RELEASE) {
    keys[key] = false;
  }
}

double PlayerView::getDx() {
  double tdx = dx;
  dx = 0;
  return tdx;
}

double PlayerView::getDy() {
  double tdy = dy;
  dy = 0;
  return tdy;
}

void PlayerView::clbkMouse(GLFWwindow* window, double xpos, double ypos) {
  static bool mouseIn = true;
  static GLfloat lastX = 400.0f;
  static GLfloat lastY = 300.0f;

  if(mouseIn) {
    lastX = xpos;
    lastY = ypos;
    mouseIn = false;
  }

  dx = xpos - lastX;
  dy = ypos - lastY;
  lastX = xpos;
  lastY = ypos;
}

bool PlayerView::shouldCloseVariable = false;
bool PlayerView::keys[512] = {};
double PlayerView::dx = 0;
double PlayerView::dy = 0;
