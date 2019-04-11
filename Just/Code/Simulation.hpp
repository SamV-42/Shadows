#ifndef SIMULATION_HPP
#define SIMULATION_HPP

class Simulation {
public:

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
