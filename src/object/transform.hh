#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Transform {
  public:
    vec3 position;
    vec3 scale;
    // in radian
    vec3 rotation;

    mat4 getMatrix();
    mat4 getRotationMatrix();

    static Transform identity();

  private:
    Transform();
};
