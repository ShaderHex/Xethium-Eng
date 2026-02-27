#pragma once
#include "mesh/mesh.h"
#include "transform/transform.h"
#include "shader/shader.h"
#include "mesh/mesh.h"
#include "gameObject/color.h"
#include <vector>

namespace GameObject{
class GameObject {
public:
    struct Object{
        Transform::Transform transform;
        Mesh::Mesh* mesh;
    };
    void CreateCube(float x, float y, float z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, Color color);
    void Render(Shader::Shader* shader);

private:
    std::vector<Object> m_cubeObjects;
    Mesh::Mesh* cubeMesh;
};
}
