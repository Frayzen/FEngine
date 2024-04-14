#pragma once

#include "object/transform.hh"
#include <glm/fwd.hpp>
class Camera {
  public:
    Transform transform = Transform::identity();
    mat4 getMatrix();

    Camera(float fov = glm::radians(45.0f), float near = 0.1f,
           float far = 100.0f);

  private:
    float fov_, near_, far_;
};

static Camera mainCamera;
