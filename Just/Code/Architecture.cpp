#include "Architecture.h"
#include "Logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const std::string Architecture::texturePath = "Assets/";


bool Architecture::shouldClose() {
  return false;
}

std::vector<std::string> Architecture::readFile(const char* target) {
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

std::vector<std::string> Architecture::splitLine(std::string line) {
  std::vector<std::string> values;
  std::stringstream splitline(line);
  std::string split;
  int i = 0;
  while(std::getline(splitline, split, ' ')) {
    values.push_back(split);
  }
  return values;
}

double Architecture::getDt() {
  return mDt;
}

void Architecture::updateTimer() {
  double tempCurrentTime = glfwGetTime();
  mDt = tempCurrentTime - mCurrentTime;
  mCurrentTime = tempCurrentTime;
}

void Architecture::resetTimer() {
  mCurrentTime = glfwGetTime();
  mDt = 0;
}

Architecture::Architecture() {
  //nothing for now...
}
