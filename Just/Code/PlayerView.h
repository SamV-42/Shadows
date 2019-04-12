#ifndef PLAYERVIEW_HPP
#define PLAYERVIEW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class PlayerView {
public:
  void initialize() {
    initInitializeOpenGL(); //these should all probably go through Architecture somehow
    initLoadFiles();        //  since they touch the actual hardware, at the very least
    initInputOutput();      //  through file I/O if not also through openGL in general

    initBufferData();
    //note that for now, we're just going to try to keep everything in memory
    //so no more streaming in new significant data, it's all in the heap or buffers somewhere
  }

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

  bool slow = false;


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
