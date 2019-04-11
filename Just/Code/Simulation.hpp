#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "Logging.hpp"
#include "Architecture.hpp"
#include "Physics/AABB.hpp"
#include "Physics/Octree.h"

class Simulation {
public:

  struct SimulationPlayerInput {
    double percentForward; // -1 to 1
    double angle; // world-relative, measured in radians
  };
  SimulationPlayerInput mInput1;

  void initialize() {
    auto AABBList = Architecture::getInstance() .readFile("GameData/AABBList.txt");
    double values[6] = {};
    for(auto& line : AABBList) {
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

  void update() {
    //<<<>>> Update all entities, handle collisions, etc.
    //Store relevant data in a playerView-accessible place somehow


  }


private:

  Simulation() {

  }

  const double LEVEL_SIZE = 25;

  Octree * mOctree;
  std::vector<std::shared_ptr<AABB> > mAABBList;
  //Don't touch post-initialization! This is meant for the unchanging bits of collision geometry
  //   and pointers to its contents will be invalidated otherwise.

//-----------------------------------------------------------
//Singleton implementation below:
//-----------------------------------------------------------

public:
  static Simulation & getInstance() {
    static Simulation instance;
    return instance;
  }

private:
  Simulation(Simulation const&);
  void operator=(Simulation const&);
};

#endif
