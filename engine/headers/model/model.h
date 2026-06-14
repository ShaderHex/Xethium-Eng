#pragma once
#include "shader/shader.h"
#include "mesh/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

namespace XENGINE{

class Model {
public:
    Model(const char *path) {
        loadModel(path);
    }
    void Draw(Shader::Shader& shader);
private:
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh::IMesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Mesh::IMesh> meshes;
    std::string directory;
};

}
