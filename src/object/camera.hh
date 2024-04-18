#pragma once

#include "object/transform.hh"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
class Camera {
  public:
    Transform transform = Transform::identity();
    mat4 getMatrix();

    Camera(float fov = glm::radians(45.0f), float near = 0.1f,
           float far = 100.0f);
    void inputs(GLFWwindow *win);
    static Camera& mainCamera();

  private:
    float fov_, near_, far_;
    float sensitivity = 100.0f;
    float speed = 1.0f;
};
