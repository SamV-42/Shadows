#include "Mesh.h"

#include <cstring>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model {
public:
    Model(std::string path);
    void Draw(Shader* shader);

    std::vector<Mesh> mMeshes;
    std::string directory;
private:
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

GLuint TextureFromFile(std::string name, std::string directory);
