#ifndef SIMULATION_HPP
#define SIMULATION_HPP

class Simulation {
public:

  struct SimulationPlayerInput {
    double percentForward; // -1 to 1
    double angle; // world-relative, measured in radians
  };
  SimulationPlayerInput mInput1;

  void initialize() {
    //<<<>>> load AABBs/etc.
  }

  void update() {
    //<<<>>> Update all entities, handle collisions, etc.
    //Store relevant data in a playerView-accessible place somehow


  }


private:

  Simulation() {

  }

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
