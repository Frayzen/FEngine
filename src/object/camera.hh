#pragma once

#include "object/transform.hh"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
class Camera {
  public:
    Transform transform = Transform::identity();
    mat4 getMatrix();

    Camera(float fov = 90.0f, float near = 0.1f,
           float far = 10000.0f);
    void inputs(GLFWwindow *win);
    static Camera& mainCamera();

  private:
    // fov in degrees
    float fov_, near_, far_;
    float sensitivity = 3.0f;
    float speed = 0.3f;

    float rotX_ = 0;
    float rotY_ = 0;
};
