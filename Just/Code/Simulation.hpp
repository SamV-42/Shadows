#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Code/Physics/AABB.hpp"
#include "Code/Physics/Octree.

#include "Code/Logging.hpp"

class Simulation {
public:

  struct SimulationPlayerInput {
    double percentForward; // -1 to 1
    double angle; // world-relative, measured in radians
  };
  SimulationPlayerInput mInput1;

  void initialize() {
    std::string line;
    std::ifstream AABBListStream("GameData/AABBList.txt");
    if( ! AABBListStream.is_open()) {
      ERROR_LOG << "ERROR::SIMULATION::INITIALIZE::FILE NOT FOUND" << std::endl;
      throw 3;
    }
    while( std::getline(AABBListStream, line) ) {
      std::stringstream splitline(line);
      std::string split;
      double values[6] = {};
      int i = 0;
      while(std::getline(splitline, split, ' ')) {
        values[i++] = std::stod(split);
      }
      mAABBList.push_back(AABB(values[0], values[1], values[2], values[3], values[4], values[5]));
    }
    AABBListStream.close();

    AABB levelBoundary(0,0,0,LEVEL_SIZE, LEVEL_SIZE, LEVEL_SIZE);
    const int maxContents = 5;  //highly fiddley value
    const int maxDepth = 3;     //also highly fiddley?
    mOctree = new Octree(nullptr, levelBoundary, maxContents, maxDepth);
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
  std::vector<AABB> mAABBList;
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
