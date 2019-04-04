#include <iostream>

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices_, std::vector<GLuint> indices_, std::vector<Texture> textures_) {
    vertices = vertices_;
    indices = indices_;
    textures = textures_;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal) );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords) );
        glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader) {
    int diffuseNr = 0;
    int specularNr = 0;
    for(int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE1 + i);
        int number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse") {
            number = diffuseNr++;
        } else if(name == "texture_specular") {
            number = specularNr++;
        }
        glUniform1i(glGetUniformLocation(shader->getProgram(), ("material" + std::to_string(number) + "." + name).c_str() ), (i+1));   //"material." +
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(0);

    //Make sure the proper shader is enabled -- do this wherever this is being called from
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::testing_updateVertices() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    //std::cout << vertices[0].Position[0] << " " << vertices[0].Position[1] << " " << vertices[0].Position[2] << std::endl;
    glBindVertexArray(0);
}
