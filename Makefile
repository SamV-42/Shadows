OF = build

final : $(OF)/dynamo.o $(OF)/Camera.o $(OF)/Shader.o $(OF)/Model.o $(OF)/Mesh.o
	g++ -o final -l GL -l SOIL -l GLEW -l glfw -l assimp $(OF)/dynamo.o $(OF)/Camera.o $(OF)/Shader.o $(OF)/Model.o $(OF)/Mesh.o

$(OF)/Camera.o : Camera.cpp Camera.h
	g++ -g -c -o $(OF)/Camera.o Camera.cpp

$(OF)/Shader.o : Shader.cpp Shader.h
	g++ -g -c -o $(OF)/Shader.o Shader.cpp

$(OF)/dynamo.o : dynamo.cpp Shader.h Camera.h
	g++ -g -c -o $(OF)/dynamo.o dynamo.cpp

$(OF)/Model.o : Model.cpp Model.h Mesh.h
	g++ -g -c -o $(OF)/Model.o Model.cpp

$(OF)/Mesh.o : Mesh.cpp Mesh.h
	g++ -g -c -o $(OF)/Mesh.o Mesh.cpp
