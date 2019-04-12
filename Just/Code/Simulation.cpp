#include "Simulation.h"
#include "Logging.hpp"
#include "Architecture.h"
#include "Physics/AABB.hpp"

#include <string>
#include <sstream>
#include <iostream>

void Simulation::initialize() {
  auto AABBFile = Architecture::getInstance() .readFile("GameData/AABBList.txt");
  double values[6] = {};
  for(auto& line : AABBFile) {
    std::stringstream splitline(line);
    std::string split;
    int i = 0;
    while(std::getline(splitline, split, ' ')) {
      values[i++] = std::stod(split);
    }
    mAABBList.push_back(std::make_shared<AABB>(values[0], values[1], values[2], values[3], values[4], values[5]));
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
