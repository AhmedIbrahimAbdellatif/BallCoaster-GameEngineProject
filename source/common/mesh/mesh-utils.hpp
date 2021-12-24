#pragma once

#include "mesh.hpp"

namespace our::mesh_utils {
    // Load an ".obj" file into the mesh
    Mesh* loadOBJ(const char* filename);
}