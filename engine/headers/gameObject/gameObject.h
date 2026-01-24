#pragma once
#include "mesh/mesh.h"
#include "transform/transform.h"

namespace GameObject{
class GameObject {
public:
    struct Object{
        Transform::Transform transform;
        Mesh::Mesh* mesh;
    };
};
}