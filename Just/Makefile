
SF = Code
OF = Build

final : $(OF)/Main.o $(OF)/Octree.o
	g++ -o final -l GL -l SOIL -l GLEW -l glfw -l assimp $(OF)/Main.o $(OF)/Octree.o

$(OF)/Main.o : $(SF)/Main.cpp $(SF)/Architecture.hpp $(SF)/Simulation.hpp $(SF)/PlayerView.hpp $(SF)/Logging.hpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -o $(OF)/Main.o $(SF)/Main.cpp

$(OF)/Octree.o : $(SF)/Physics/Octree.cpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -o $(OF)/Octree.o $(SF)/Physics/Octree.cpp
