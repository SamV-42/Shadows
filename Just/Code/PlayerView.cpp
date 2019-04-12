#include <cmath>
#include <string>
#include <sstream>

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

const GLuint WIDTH = 400;
const GLuint HEIGHT = 300;


void PlayerView::initialize() {
  //these should all probably go through Architecture somehow
  //  since they touch the actual hardware, at the very least
   //  through file I/O if not also through openGL in general

  initInitializeOpenGL();
  initLoadFiles();        //shaders
  initBufferData();       //models
  initInputOutput();      //
  //note that for now, we're just going to try to keep everything in memory
  //so no more streaming in new significant data, it's all in the heap or buffers somewhere
}

bool PlayerView::shouldClose() {
  return glfwWindowShouldClose(mWindow);
}

void PlayerView::respondToEvents() {
  glfwPollEvents();

  double fb = keys[GLFW_KEY_W] - keys[GLFW_KEY_S];
  double rl = keys[GLFW_KEY_D] - keys[GLFW_KEY_A];

  double speed = (1 - mSlow * 0.5) * (fb == rl ? abs(fb) : 1);
  double angle = atan2(rl, fb);  //reversed so forward is 0, clockwise is positive

  Simulation::getInstance() .mInput1.percentForward = speed;
  Simulation::getInstance() .mInput1.angle = angle;
}

void PlayerView::updateView() {
  //<<<>>> Display things!
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

void PlayerView::initLoadFiles() {
  auto shaderFile = Architecture::getInstance() .readFile("GameData/ShaderList.txt");
  std::string values[3] = {};
  const std::string shader_location("Code/Shader/Shaders/");
  for(auto& line : shaderFile) {
    std::stringstream splitline(line);
    std::string split;
    int i = 0;
    while(std::getline(splitline, split, ' ')) {
      values[i++] = shader_location + split;
    }
    values[2] = ( (values[2] == shader_location + "NO_PATH") ? "NO_PATH" : values[2] );
    mShaders.push_back(Shader(values[0], values[1], values[2]));
  }

  std::string ubo_lists[3] = {"Matrices", "viewPos", "pointLights"};
  for(int i = 0;    i < 3; ++i) {
    for(auto& shader : mShaders) {
      GLuint index = glGetUniformBlockIndex(shader.getProgram(), ubo_lists[i].c_str() );
      glUniformBlockBinding(shader.getProgram(), index, i);
    }
  }
}

void PlayerView::initBufferData() {
  auto modelsFile = Architecture::getInstance() .readFile("GameData/ModelList.txt");
  GLfloat values[10] = {};
  const std::string models_location("Assets/");
  for(auto& line : modelsFile) {
    std::stringstream splitline(line);
    std::string split;
    for(int i = 0; i < 10; ++i) {
      std::getline(splitline, split, ' ');
      values[i] = std::stod(split);
    }
    std::getline(splitline, split, ' ');
    std::string modelPath = "Assets/" + split;

    glm::mat4 initialModelMtx = glm::mat4(1.0f);
    initialModelMtx = glm::scale(initialModelMtx, glm::vec3(values[7], values[8], values[9]));
    initialModelMtx = glm::rotate(initialModelMtx, glm::radians(values[3]), glm::vec3(values[4], values[5], values[6]));
    initialModelMtx = glm::translate(initialModelMtx, glm::vec3(values[0], values[1], values[2]));

    ModelWrapper model = ModelWrapper(modelPath);
    model.setModelMtx(initialModelMtx);
    mModels.push_back(model);
  }
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

bool PlayerView::keys[512] = {};
double PlayerView::dx = 0;
double PlayerView::dy = 0;
