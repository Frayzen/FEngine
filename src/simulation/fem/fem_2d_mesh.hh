#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <glm/ext/vector_uint3.hpp>
#include "mesh/mesh.hh"
#include "object/object.hh"
#include <vector>

using namespace glm;

enum FEMFlag { NONE, FIXED, ROLLING_X, ROLLING_Y };

struct FEMPoint {
    vec3 coord;
    FEMFlag flags;
    vec3 forceApplied;
};

class FEM2DMesh {

  public:
    FEM2DMesh();
    void compute(void);
    void add_beam(vec3 v1, vec3 v2);
    void updatePos(int id);
    void reset();

    Mesh &getMesh(void);

  private:
    Mesh beam_;
    std::vector<uvec2> elems1d_;
    std::vector<FEMPoint> elems0d_;
};
