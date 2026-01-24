#pragma once
#include "mesh/mesh.h"

namespace MeshFactory {
class MeshFactory {
public:
    static Mesh::Mesh* CreateCube();
};
}
