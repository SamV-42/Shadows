final : dynamo.o Camera.o Shader.o Model.o Mesh.o
	g++ -o final -l GL -l SOIL -l GLEW -l glfw -l assimp dynamo.o Camera.o Shader.o Model.o Mesh.o

Camera.o : Camera.cpp Camera.h
	g++ -g -c -o Camera.o Camera.cpp

Shader.o : Shader.cpp Shader.h
	g++ -g -c -o Shader.o Shader.cpp

dynamo.o : dynamo.cpp Shader.h Camera.h
	g++ -g -c -o dynamo.o dynamo.cpp

Model.o : Model.cpp Model.h Mesh.h
	g++ -g -c -o Model.o Model.cpp

Mesh.o : Mesh.cpp Mesh.h
	g++ -g -c -o Mesh.o Mesh.cpp
