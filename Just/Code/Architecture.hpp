#ifndef ARCHITECTURE_HPP
#define ARCHITECTURE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Architecture {
public:

    bool shouldClose() {
      return false;
    }

    std::vector<std::string> readFile(const char* target) {
      std::vector<std::string> output;
      std::string line;
      std::ifstream listStream(target);
      if( ! listStream.is_open()) {
        ERROR_LOG << "ERROR::ARCHITECTURE::READFILE::FILE NOT FOUND: " << target << std::endl;
        throw 3;
      }
      while( std::getline(listStream, line) ) {
        output.push_back(line);
      }
      listStream.close();
      return output;
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
