#pragma once

#include "object/camera.hh"
#include "object/object.hh"
#include <glm/ext/vector_float3.hpp>

using namespace glm;

class Ray {
  public:
    static Ray fromClick(Camera &cam, vec2 &where);
    Ray(vec3 org, vec3 dir);
    bool intersect(Object &o, vec3 &pos);
    bool intersect(std::vector<Object> &objs, int &outId, vec3 &pos);
    vec3 org, dir, dirfrac;
};
