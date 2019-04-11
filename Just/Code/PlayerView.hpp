#ifndef PLAYERVIEW_HPP
#define PLAYERVIEW_HPP

#define GLEW_STATIC
#include <GL/glew.h>

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLuint WIDTH = 1200;
const GLuint HEIGHT = 800;

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

    //<<<>>> Put I/O into a place in Simulation
  }

  void updateView() {
    //<<<>>> Display things!
  }

  void shutdown() {
    glfwTerminate();
  }

private:

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

  static void clbkMouse(GLFWwindow* window, double xpos, double ypos) {
    getInstance();
    //<<<>>>
  }

  static void clbkKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
    getInstance();
    //<<<>>>
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



  PlayerView() { }

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
