#pragma once
#include "mesh/mesh.h"
#include "../../include/defines.h"

namespace MeshFactory {
class MeshFactory {
public:
    XAPI static Mesh::Mesh* CreateCube();
    static Mesh::Mesh* CreateQuadMesh();
};
}
