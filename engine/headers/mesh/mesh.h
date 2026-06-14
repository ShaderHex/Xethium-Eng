#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "shader/shader.h"

namespace Mesh{

struct Mesh {
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int vertexCount = 0;
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoords;
};

class IMesh {
public:
    IMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    void Draw(Shader::Shader& shader);
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
private:
    void setupMesh();

    unsigned int m_VBO, m_VAO, m_EBO;
};

}