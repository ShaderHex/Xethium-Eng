#include "model/model.h"
#include "mesh/mesh.h"

#include <iostream>

namespace XENGINE {

void Model::Draw(Shader::Shader& shader) {
    for(unsigned int i = 0; i < meshes.size(); i++) 
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string path) {
    std::cout<< "[Model] Started loading the model onto memory\n";
    Assimp::Importer Importer;
    std::cout<< "[Model] Started loading from file\n";
    const aiScene *scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    std::cout<< "[Model] Finished loading from file\n";
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    std::cout<< "[Model] Processing the nodes\n";
    processNode(scene->mRootNode, scene);
    std::cout<< "[Model] Finished processing the nodes\n";
    std::cout<< "[Model] Finished loading the model onto memory\n";
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    int totalVertices = 0;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // totalVertices += 
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        std::cout<< "[Model] Vertices: " << mesh->mNumVertices << "\n";
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh::IMesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
        
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // vertex.Position.x *= 50.0f;
        //     std::cout << "Pos: " 
        //   << vertex.Position.x << ", "
        //   << vertex.Position.y << ", "
        //   << vertex.Position.z << std::endl;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        vertices.push_back(vertex);

    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
        return Mesh::IMesh(vertices, indices);
}

}