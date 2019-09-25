#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader/Shader.h"
#include "Display/ModelWrapper.h"
#include "Camera/TargetCamera.h"


class PlayerView {
public:

  void initialize();

  bool shouldClose();

  void respondToEvents();

  void updateView();

  void shutdown();

  ModelWrapper * mPlayer;
  BaseCamera * camera;
  //  we need a longer-term solution for how Simulation can move things, but for now just the player is fine

private:

  PlayerView();

//-----------------------------------------------------------------------------------
//Initialization

  GLFWwindow* mWindow;

  void initInitializeOpenGL();

  void initLoadShaders();

  void initLoadModels();

  void initBufferData();

  void initInputOutput();

  void initFramebuffers();

  void initShameful();

//-----------------------------------------------------------------------------------
//Input

  static bool keys[512]; //WASD, respectively

  static void clbkKey(GLFWwindow* window, int key, int scancode, int action, int mode);

  static double dx, dy;

  static double getDx();

  static double getDy();

  static void clbkMouse(GLFWwindow* window, double xpos, double ypos);

  static bool shouldCloseVariable;

  //-----------------------------------------------------------------------------------
  //Display

  bool mSlow = false;

  //BaseCamera * camera;

  std::vector<Shader> mShaders;
  std::vector<GLuint> mUniformBuffers;
  std::vector<std::vector<ModelWrapper> > mModels;  //draw models with the same shader together
  const int maxPointLights = 10;
  std::vector<glm::vec3> mPointLightsList;

  GLuint fboMultisample, fboIntermediate;
  GLuint outputTexture;

  //-----------------------------------------------------------------------------------
  //Shameful -- anything that ought to be handled more elegantly at some point in the future

  GLuint stupidMeshVAO;
  void setupStupidMesh();
  GLuint cubemapTexture;
  GLuint cubemapVAO;
  void setupCubemap();

//-----------------------------------------------------------
//Singleton implementation below:
//-----------------------------------------------------------

public:
  static PlayerView & getInstance() {
    static PlayerView instance;
    return instance;
  }

private:
  PlayerView(PlayerView const&);
  void operator=(PlayerView const&);
};

#endif
