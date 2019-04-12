SF = Code
OF = Build

final : $(OF)/Main.o $(OF)/Octree.o $(OF)/PlayerView.o $(OF)/Architecture.o $(OF)/Simulation.o
	g++ -o final -l GL -l SOIL -l GLEW -l glfw -l assimp $(OF)/Main.o $(OF)/Octree.o $(OF)/PlayerView.o $(OF)/Architecture.o $(OF)/Simulation.o

$(OF)/Main.o : $(SF)/Main.cpp $(SF)/Architecture.h $(SF)/Simulation.h $(SF)/PlayerView.h $(SF)/Logging.hpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -o $(OF)/Main.o $(SF)/Main.cpp

$(OF)/Octree.o : $(SF)/Physics/Octree.cpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -o $(OF)/Octree.o $(SF)/Physics/Octree.cpp

$(OF)/PlayerView.o : $(SF)/PlayerView.cpp $(SF)/PlayerView.h $(SF)/Logging.hpp $(SF)/Simulation.h
	g++ -g -c -o $(OF)/PlayerView.o $(SF)/PlayerView.cpp

$(OF)/Simulation.o : $(SF)/Simulation.cpp $(SF)/Simulation.h $(SF)/Logging.hpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -o $(OF)/Simulation.o $(SF)/Simulation.cpp

$(OF)/Architecture.o : $(SF)/Architecture.cpp $(SF)/Architecture.h
	g++ -g -c -o $(OF)/Architecture.o $(SF)/Architecture.cpp
