#pragma once

#include "object/ray.hh"
#include <glm/ext/vector_float3.hpp>

using namespace glm;

class AABBox {
  public:
    static AABBox noBounds(void);
    void update(const vec3 &v);
    AABBox(vec3 min, vec3 max);
    bool intersect(Ray &r, vec3 offset);

    vec3 min_, max_;
  private:
};
