#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Vertex.hpp"
#include "Texture.h"
#include "Shader/Shader.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> & vertices_, std::vector<GLuint> & indices_, std::vector<Texture> & textures_);

    void Draw(Shader* shader);

//private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

#endif
