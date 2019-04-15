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

const GLuint WIDTH = 400;
const GLuint HEIGHT = 300;


void PlayerView::initialize() {
  //these should all probably go through Architecture somehow
  //  since they touch the actual hardware, at the very least
   //  through file I/O if not also through openGL in general

  initInitializeOpenGL();
  initLoadShaders();
  initLoadModels();
  initBufferData();      //a few uniforms/UBOs, Camera
  initFramebuffers();
  //note that for now, we're just going to try to keep everything in memory
  //so no more streaming in new significant data, it's all in the heap or buffers somewhere
}

bool PlayerView::shouldClose() {
  return glfwWindowShouldClose(mWindow) || shouldCloseVariable;
}

void PlayerView::respondToEvents() {
  glfwPollEvents();

  shouldCloseVariable = keys[GLFW_KEY_ESCAPE];

  double fb = keys[GLFW_KEY_S] - keys[GLFW_KEY_W];
  double rl = keys[GLFW_KEY_D] - keys[GLFW_KEY_A];

  double speed = (1 - mSlow * 0.5) * (fb == rl ? abs(fb) : 1);
  double angle = atan2(rl, fb);  //reversed so forward is 0, clockwise is positive

  Simulation::getInstance() .mInput1.percentForward = speed;
  Simulation::getInstance() .mInput1.angle = angle;
}

//BEACON
void PlayerView::updateView() {
  glBindFramebuffer(GL_FRAMEBUFFER, fboMultisample);

  glClearColor(0.4f, 0.1f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  for(int i = 0; i < mModels.size(); ++i) {
    Shader & shader = mShaders.at(i);
    shader.use();

    glUniform1f(glGetUniformLocation(shader.getProgram(), "material0.shininess"), 64);

    for(auto& model : mModels.at(i)) {
      shader.sendMatrix4fv(ShaderListEnum::model, model.getModelMtx());

      model.draw(&shader);
    }
  }


  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMultisample);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboIntermediate);
  glBlitFramebuffer(0,0,WIDTH, HEIGHT, 0,0,WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  mShaders.at(1).use();

  glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);


  glActiveTexture(GL_TEXTURE0);
  mShaders.at(1).send1i(ShaderListEnum::texture1, 0);   //not actually needed
  glBindVertexArray(stupidMeshVAO);
  glBindTexture(GL_TEXTURE_2D, outputTexture);
  glDrawElements( GL_TRIANGLES, 6,  GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

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

  Shader::initUBOBuffers();

  Shader::initUBO(ShaderListUBOEnum::FragmentData, sizeof(glm::vec3) );
  Shader::loadUBO(ShaderListUBOEnum::FragmentData, camera->getCameraPos(), 0);

  Shader::initUBO(ShaderListUBOEnum::Matrices, 2*sizeof(glm::mat4) );
  Shader::loadUBO(ShaderListUBOEnum::Matrices, camera->getProj(), 0);
  Shader::loadUBO(ShaderListUBOEnum::Matrices, camera->getView(), sizeof(glm::mat4));

  const int maxPointLights = 10;
  std::vector<glm::vec3> pointLightsList {  //new vector declaration in C++11
  		glm::vec3(  5, 0, 7  ),
  		glm::vec3(  2, 0, 3  ),
  		glm::vec3(  4, 0, -6  ),
  		glm::vec3(  0, 0, 8  )
  	};

  //Note: All the below offsets are predicted by std140. And verified by me after much frustration.
  std::size_t pointLightSize = 80;
  Shader::initUBO(ShaderListUBOEnum::PointLights, 850 );  //little extra in case
  for(int i = 0; i < pointLightsList.size(); ++i) {
    Shader::loadUBO(ShaderListUBOEnum::PointLights, pointLightsList[i], i*pointLightSize);

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lightAmbient = lightColor * glm::vec3(0.05f);
    glm::vec3 lightDiffuse = lightColor * glm::vec3(0.5f);
    glm::vec3 lightSpecular = lightColor;

    Shader::loadUBO(ShaderListUBOEnum::PointLights, lightAmbient,  16 + i*pointLightSize);
    Shader::loadUBO(ShaderListUBOEnum::PointLights, lightDiffuse,  32 + i*pointLightSize);
    Shader::loadUBO(ShaderListUBOEnum::PointLights, lightSpecular, 48 + i*pointLightSize);

    GLfloat atn_c = 1.0f;
    GLfloat atn_l = 0.05f;
    GLfloat atn_q = 0.001f;

    Shader::loadUBO(ShaderListUBOEnum::PointLights, atn_c, 60 + i*pointLightSize);
    Shader::loadUBO(ShaderListUBOEnum::PointLights, atn_l, 64 + i*pointLightSize);
    Shader::loadUBO(ShaderListUBOEnum::PointLights, atn_q, 68 + i*pointLightSize);
  }
  Shader::loadUBO(ShaderListUBOEnum::PointLights, (GLint)(pointLightsList.size()), 10*pointLightSize);
}

void PlayerView::initFramebuffers() {
  glGenFramebuffers(1, &fboMultisample);
  glBindFramebuffer(GL_FRAMEBUFFER, fboMultisample);

  GLuint fbotext1;
  glGenTextures(1, &fbotext1);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbotext1);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, WIDTH, HEIGHT, GL_TRUE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbotext1, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    ERROR_LOG << "ERROR::PLAYERVIEW::initFramebuffers FBO 1 failed to complete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenFramebuffers(1, &fboIntermediate);
  glBindFramebuffer(GL_FRAMEBUFFER, fboIntermediate);

  glGenTextures(1, &outputTexture);
  glBindTexture(GL_TEXTURE_2D, outputTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    ERROR_LOG << "ERROR::PLAYERVIEW::initFramebuffers FBO 2 failed to complete" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  setupStupidMesh();  //I swear I'll put this in data at some point
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


void PlayerView::setupStupidMesh() {
  mShaders.at(1).use();
  glGenVertexArrays(1, &stupidMeshVAO);
  GLuint VBO; glGenBuffers(1, &VBO);
  GLuint EBO; glGenBuffers(1, &EBO);

  GLfloat vertices[] = {-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
  GLuint indices[] = { 0, 2, 1, 0, 3, 2 };

  glBindVertexArray(stupidMeshVAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (GLvoid*)0);
      glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}
