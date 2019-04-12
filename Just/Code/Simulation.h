#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <memory>

#include "Physics/Octree.h"

class Simulation {
public:

  struct SimulationPlayerInput {
    double percentForward; // -1 to 1
    double angle; // world-relative, measured in radians
  };
  SimulationPlayerInput mInput1;

  void initialize();

  void update();


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
