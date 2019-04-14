#include "Simulation.h"
#include "Logging.hpp"
#include "Architecture.h"
#include "Physics/AABB.hpp"

#include <string>
#include <sstream>
#include <iostream>

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
  //<<<>>> Update all entities, handle collisions, etc.
  //Store relevant data in a playerView-accessible place somehow
}
