#ifndef ARCHITECTURE_HPP
#define ARCHITECTURE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Architecture {
public:

    bool shouldClose() {
      return false;
    }

    //timer
    //Important note: Timer should be used after PlayerView is initialized
    //The benefit of using glfw's cross-platform timer outweights the interconnection

    double getDt() {
      return mDt;
    }

    void updateTimer() {
      double tempCurrentTime = glfwGetTime();
      mDt = tempCurrentTime - mCurrentTime;
      mCurrentTime = tempCurrentTime;
    }

    void resetTimer() {
      mCurrentTime = glfwGetTime();
      mDt = 0;
    }

private:

  double mCurrentTime;
  double mDt;

  Architecture() {
    //nothing for now...
  }

//-----------------------------------------------------------
//Singleton implementation below:
//-----------------------------------------------------------

public:
  static Architecture & getInstance() {
    static Architecture instance;
    return instance;
  }

private:
  Architecture(Architecture const&);
  void operator=(Architecture const&);
};


#endif
