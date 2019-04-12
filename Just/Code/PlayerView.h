#ifndef PLAYERVIEW_HPP
#define PLAYERVIEW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader/Shader.h"
#include "Display/ModelWrapper.h"


class PlayerView {
public:

  void initialize();

  bool shouldClose();

  void respondToEvents();

  void updateView();

  void shutdown();

private:

  PlayerView();

//-----------------------------------------------------------------------------------
//Initialization

  GLFWwindow* mWindow;

  void initInitializeOpenGL();

  void initLoadFiles();

  void initBufferData();

  void initInputOutput();

//-----------------------------------------------------------------------------------
//Input

  static bool keys[512]; //WASD, respectively

  static void clbkKey(GLFWwindow* window, int key, int scancode, int action, int mode);

  static double dx, dy;

  static double getDx();

  static double getDy();

  static void clbkMouse(GLFWwindow* window, double xpos, double ypos);

  //-----------------------------------------------------------------------------------
  //Display

  bool mSlow = false;

  std::vector<Shader> mShaders;
  std::vector<GLuint> mUniformBuffers;
  std::vector<ModelWrapper> mModels;


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
