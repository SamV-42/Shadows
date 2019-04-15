#include "Simulation.h"
#include "Logging.hpp"
#include "Architecture.h"
#include "Physics/AABB.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

void Simulation::initialize() {
  auto AABBFile = Architecture::getInstance() .readFile("GameData/AABBList.txt");

  for(auto& line : AABBFile) {
    std::vector<std::string> values = Architecture::getInstance() .splitLine(line);
    mAABBList.push_back(std::make_shared<AABB>(std::stod(values[0]), std::stod(values[1]), std::stod(values[2]), std::stod(values[3]), std::stod(values[4]), std::stod(values[5])));
  }

  AABB levelBoundary(0,0,0,LEVEL_SIZE, LEVEL_SIZE, LEVEL_SIZE);
  const int maxContents = 5;  //highly fiddley value
  const int maxDepth = 3;     //also highly fiddley?
  mOctree = new Octree(nullptr, levelBoundary, maxContents, maxDepth);

  for(auto& aabb_ptr: mAABBList) {
    mOctree->addMember(aabb_ptr);
  }

}

void Simulation::update() {

  const double sensitivity = 5.0f;

  static double pitch = 0;
  static double yaw = 0;

  yaw += sensitivity * mInput1.dx * Architecture::getInstance().getDt();
  pitch -= sensitivity * mInput1.dy * Architecture::getInstance().getDt();
  if(pitch > 89.0f) pitch = 89.0f; if(pitch < -89.0f) pitch = -89.0f;
  glm::vec3 direction;
  direction.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
  direction.y = sin(glm::radians(pitch));
  direction.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
  direction = glm::normalize(direction);
  PlayerView::getInstance() .camera->setCameraFront(direction);

  direction.y = 0;
  glm::vec3 strafeDir = glm::vec3(direction.z, 0.0f, -direction.x);
  direction *= mInput1.percentForward;
  strafeDir *= mInput1.percentStrafe;

  direction = direction + strafeDir;
  if(mInput1.percentForward || mInput1.percentStrafe) {
    direction = glm::normalize(direction);
  }
  direction *= 10.0f * Architecture::getInstance().getDt();
  glm::vec3 cameraPos = PlayerView::getInstance() .camera->getCameraPos();
  cameraPos += direction;

  AABB playerAABB(cameraPos.x, cameraPos.y, cameraPos.z, 0.35f, 1.0f, 0.12f);

  if(mOctree->checkCollision(playerAABB) != nullptr) {
    cameraPos -= direction;
  }

  PlayerView::getInstance() .camera->setCameraPos(cameraPos);

  //<<<>>> Update all entities, handle collisions, etc.
  //Store relevant data in a playerView-accessible place somehow
}
