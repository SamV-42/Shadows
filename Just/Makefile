TP = /home/sam/Programming/Shadows/Just
SF = $(TP)/Code
OF = $(TP)/Build

final : $(OF)/Main.o $(OF)/Octree.o $(OF)/PlayerView.o $(OF)/Architecture.o $(OF)/Simulation.o $(OF)/Shader.o $(OF)/ModelWrapper.o $(OF)/Model.o $(OF)/Mesh.o $(OF)/Texture.o
	g++ -o final -l GL -l SOIL -l GLEW -l glfw -l assimp $(OF)/Main.o $(OF)/Octree.o $(OF)/PlayerView.o $(OF)/Architecture.o $(OF)/Simulation.o $(OF)/Shader.o $(OF)/ModelWrapper.o $(OF)/Model.o $(OF)/Mesh.o $(OF)/Texture.o

$(OF)/Main.o : $(SF)/Main.cpp $(SF)/Architecture.h $(SF)/Simulation.h $(SF)/PlayerView.h $(SF)/Logging.hpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -I $(SF) -o $(OF)/Main.o $(SF)/Main.cpp

$(OF)/Octree.o : $(SF)/Physics/Octree.cpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -I $(SF) -o $(OF)/Octree.o $(SF)/Physics/Octree.cpp

$(OF)/PlayerView.o : $(SF)/PlayerView.cpp $(SF)/PlayerView.h $(SF)/Logging.hpp $(SF)/Simulation.h $(SF)/Shader/Shader.h $(SF)/Display/ModelWrapper.h
	g++ -g -c -I $(SF) -o $(OF)/PlayerView.o $(SF)/PlayerView.cpp

$(OF)/Simulation.o : $(SF)/Simulation.cpp $(SF)/Simulation.h $(SF)/Logging.hpp $(SF)/Physics/Octree.h $(SF)/Physics/AABB.hpp $(SF)/Physics/Point.hpp
	g++ -g -c -I $(SF) -o $(OF)/Simulation.o $(SF)/Simulation.cpp

$(OF)/Architecture.o : $(SF)/Architecture.cpp $(SF)/Architecture.h
	g++ -g -c -I $(SF) -o $(OF)/Architecture.o $(SF)/Architecture.cpp

$(OF)/Shader.o : $(SF)/Shader/Shader.cpp $(SF)/Shader/Shader.h $(SF)/Logging.hpp
	g++ -g -c -I $(SF) -o $(OF)/Shader.o $(SF)/Shader/Shader.cpp

$(OF)/ModelWrapper.o : $(SF)/Display/ModelWrapper.cpp $(SF)/Display/ModelWrapper.h $(SF)/Display/Model.h
	g++ -g -c -I $(SF) -o $(OF)/ModelWrapper.o $(SF)/Display/ModelWrapper.cpp

$(OF)/Model.o : $(SF)/Display/Model.cpp $(SF)/Display/Model.h $(SF)/Display/Mesh.h $(SF)/Display/Texture.h
	g++ -g -c -I $(SF) -o $(OF)/Model.o $(SF)/Display/Model.cpp

$(OF)/Mesh.o : $(SF)/Display/Mesh.cpp $(SF)/Display/Mesh.h $(SF)/Display/Texture.h $(SF)/Display/Vertex.hpp
	g++ -g -c -I $(SF) -o $(OF)/Mesh.o $(SF)/Display/Mesh.cpp

$(OF)/Texture.o : $(SF)/Display/Texture.cpp $(SF)/Display/Texture.h
	g++ -g -c -I $(SF) -o $(OF)/Texture.o $(SF)/Display/Texture.cpp
