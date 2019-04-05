#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices_, std::vector<GLuint> indices_, std::vector<Texture> textures_);

    void Draw(Shader* shader, int instance_number = 1);
    void testing_updateVertices();

//private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};
#endif
