#include "mesh/mesh.h"
#include "glad/glad.h"
#include <iostream>

namespace Mesh {

IMesh::IMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
    : m_vertices(vertices), m_indices(indices)
{
    std::cout << "Vertices: " << vertices.size() << "\n";
std::cout << "Indices: " << indices.size() << "\n";
    setupMesh();
}

void IMesh::setupMesh() {
    std::cout<<"[IMESHHHHHHHHHHH] Setting up mesh!!!\n";
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                 &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
    std::cout<<"[IMESHHHHHHHHHHH] Finished Setting up mesh!!!\n";
}

void IMesh::Draw(Shader::Shader& shader) {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
}

}