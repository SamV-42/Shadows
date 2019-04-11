#ifndef PLAYERVIEW_HPP
#define PLAYERVIEW_HPP

#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLuint WIDTH = 400;
const GLuint HEIGHT = 300;

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

  bool shouldClose() {
    return glfwWindowShouldClose(mWindow);
  }

  void respondToEvents() {
    glfwPollEvents();

    double fb = keys[GLFW_KEY_W] - keys[GLFW_KEY_S];
    double rl = keys[GLFW_KEY_D] - keys[GLFW_KEY_A];

    double speed = (1 - slow * 0.5) * (fb == rl ? abs(fb) : 1);
    double angle = atan2(rl, fb);  //reversed so forward is 0, clockwise is positive

    Simulation::getInstance() .mInput1.percentForward = speed;
    Simulation::getInstance() .mInput1.angle = angle;
  }

  void updateView() {
    //<<<>>> Display things!
  }

  void shutdown() {
    glfwTerminate();
  }

private:

  PlayerView() {

  }

//-----------------------------------------------------------------------------------
//Initialization

  GLFWwindow* mWindow;

  void initInitializeOpenGL() {

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


  void initLoadFiles() {
    //<<<>>>
  }

  void initBufferData() {
    //<<<>>>
  }

  void initInputOutput() {
    //<<<>>>
  }

//-----------------------------------------------------------------------------------
//Input

  static bool keys[512]; //WASD, respectively

  static void clbkKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(action == GLFW_PRESS) {
  		keys[key] = true;
  	} else if(action == GLFW_RELEASE) {
  		keys[key] = false;
  	}
  }

  static double dx, dy;

  static double getDx() {
    double tdx = dx;
    dx = 0;
    return tdx;
  }

  static double getDy() {
    double tdy = dy;
    dy = 0;
    return tdy;
  }

  static void clbkMouse(GLFWwindow* window, double xpos, double ypos) {
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

bool PlayerView::keys[512] = {};
double PlayerView::dx = 0;
double PlayerView::dy = 0;

#endif
