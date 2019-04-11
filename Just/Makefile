
SF = Code
OF = Build

final : $(OF)/Main.o
	g++ -o final -l GL -l SOIL -l GLEW -l glfw -l assimp $(OF)/Main.o

$(OF)/Main.o : $(SF)/Main.cpp $(SF)/Architecture.hpp $(SF)/Simulation.hpp $(SF)/PlayerView.hpp $(SF)/Logging.hpp
	g++ -g -c -o $(OF)/Main.o $(SF)/Main.cpp
