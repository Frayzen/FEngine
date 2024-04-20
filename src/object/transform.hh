#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Transform {
  public:
    Transform(vec3 pos, quat rot, vec3 scale);
    vec3 position;
    quat rotation;
    vec3 scale;

    mat4 getMatrix();
    mat4 getRotationMatrix();

    static Transform identity();

    vec3 front();
    vec3 left();
    vec3 up();

    void setRotation(vec3 v);

  private:
    Transform();
};
