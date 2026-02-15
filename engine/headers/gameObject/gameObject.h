#pragma once
#include "mesh/mesh.h"
#include "transform/transform.h"
#include "shader/shader.h"
#include "mesh/mesh.h"
#include <vector>

namespace GameObject{
class GameObject {
public:
    struct Object{
        Transform::Transform transform;
        Mesh::Mesh* mesh;
    };
    void CreateCube(float x, float y, float z);
    void Render(Shader::Shader* shader);

private:
    std::vector<Object> m_cubeObjects;
    GameObject::Object cubeObject;
    Mesh::Mesh* cubeMesh;
};
}
