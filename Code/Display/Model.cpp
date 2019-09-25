#include <iostream>

//#define GLEW_STATIC
//#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Model.h"
#include "Texture.h"    //TextureFromFile

void Model::Draw(Shader* shader) {
    for(auto mesh : mMeshes) {
        mesh.Draw(shader);
    }
}

Model::Model(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR: ASSIMP failed to load a scene: " << path << " with error " << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/')); //okay I guess, whatever

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for(int i = 0; i < node->mNumMeshes; ++i) {
        mMeshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));    //node mMeshes returns an index to scene mMeshes
    }   //note that later we'll want to preserve parent child relationships for the sake of translations and such -- unless we can just express individual objects as different models? Makes things like skeletons easier, but I'm not sure I'll use that...
    for(int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    for(int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    for(int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    Mesh moosh = Mesh(vertices, indices, textures);
    return moosh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string temp(str.C_Str());
        temp = directory + "/" + temp;
        Texture texture = Texture::createTexture(temp, typeName);
        textures.push_back(texture);
    }
    return textures;
}
