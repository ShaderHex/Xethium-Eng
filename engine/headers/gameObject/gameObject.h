#pragma once
#include "mesh/mesh.h"
#include "transform/transform.h"
#include "shader/shader.h"
#include "mesh/mesh.h"
#include "gameObject/color.h"
#include "texture/texture.h"
// #include "renderer/renderer.h"
#include <memory>
#include <vector>

namespace GameObject{
class GameObject {
public:
    struct Object{
        Transform::Transform transform;
        Mesh::Mesh* mesh;
        std::shared_ptr<Texture::Texture> texture;
    };

    struct CubeSpec {
        Transform::Vector3 position = {0,0,0};
        Transform::Vector3 rotation = {0,0,0};
        Transform::Vector3 scale = {1, 1, 1};

        std::shared_ptr<Texture::Texture> texture;
    };

    GameObject::Object* CreateCube(CubeSpec CubeSpec, Color color);
    void Render(Shader::Shader* shader);
    std::vector<Object>& GetCubeObjects() { return m_cubeObjects; }
    unsigned int GetVAO() const { return m_VAO; }
    unsigned int GetVertexCount() const { return m_vertexCount; }
    
    std::vector<Object> m_cubeObjects;
private:
    Mesh::Mesh* cubeMesh;
    unsigned int m_vertexCount;
    unsigned int m_VAO;
    // Texture::Texture* m_texture;
};
}
