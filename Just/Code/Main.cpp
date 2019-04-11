
#include "Logging.hpp"

#include "Architecture.hpp"
#include "Simulation.hpp"
#include "PlayerView.hpp"

void Initialize();
bool MainLoop();
void Shutdown();

int main() {

  Initialize();

  while(MainLoop()) {} //are we using threads for anything?

  Shutdown();

}

void Initialize() {
  //TODO: Is there a way of using templates to define 'singleton classes'?
  //Architecture:  mostly handles a timer at the moment
  //Simulation: handles all physics collisions, etc.
  //PlayerView: all the rendering, also I/O

  Simulation::getInstance() .initialize();  //this should load & initialize AABBs etc. from files
  PlayerView::getInstance() .initialize();  //this should initialize opengl, load files&buffers, etc

  Architecture::getInstance() .resetTimer();
}

bool MainLoop() {
  Architecture::getInstance() .updateTimer();

  PlayerView::getInstance() .respondToEvents();   //Singleton: sets input values in Simulation

  Simulation::getInstance() .update();  //Singleton: uses the timer value in Architecture

  PlayerView::getInstance() .updateView();  //Singleton: receives output values from Simulation

  return !( Architecture::getInstance() .shouldClose() || PlayerView::getInstance() .shouldClose() );
}

void Shutdown() {

  PlayerView::getInstance() .shutdown();  //only one currently with anything to shut down

  //terrible as it may be, we're just going to let the runtime environment dump the program memory
  //TODO: actually delete remaining pointers in the program
}