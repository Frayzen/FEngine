#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <glm/ext/vector_uint3.hpp>
#include "mesh/mesh.hh"
#include "object/object.hh"
#include <vector>

using namespace glm;

class FEM2DMesh {

  public:
    FEM2DMesh();
    void add_beam(vec3 v1, vec3 v2);
    void updatePos(int id);

    Mesh &getMesh(void);

  private:
    Mesh beam_;
    std::vector<uvec2> elems1d_;
    std::vector<vec3> elems0d_;
};
