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

  glm::vec3 translation( sin(mInput1.angle)*mInput1.percentForward, 0.0f, cos(mInput1.angle)*mInput1.percentForward );
  translation *= glm::vec3(10.0f * Architecture::getInstance().getDt() );
  PlayerView::getInstance() .mPlayer->translate(translation);

  glm::vec3 currentTrans = PlayerView::getInstance() .mPlayer->getTranslation();
  //std::cout << currentTrans.x << " " << currentTrans.y << " " << currentTrans.z << std::endl;
  AABB playerAABB(currentTrans.x, currentTrans.y, currentTrans.z, 0.35f, 0.12f, 0.12f);

  //std::cout << (AABB::intersectAABB(*mAABBList[5], playerAABB) ? "yes" : "no") << std::endl;
  //mOctree->testingPrint();
  //std::cout << std::endl;

  if(mOctree->checkCollision(playerAABB) != nullptr) {
    //mOctree->checkCollision(playerAABB)->testingPrint(); std::cout << std::endl;
    //playerAABB.testingPrint();std::cout << std::endl; std::cout << std::endl;
    translation *= glm::vec3(-1);
    PlayerView::getInstance() .mPlayer->translate(translation);
  }

  //<<<>>> Update all entities, handle collisions, etc.
  //Store relevant data in a playerView-accessible place somehow
}
