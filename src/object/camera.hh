#pragma once

#include "object/transform.hh"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>

#define RCLICK 1
#define LCLICK -1
#define NOCLICK 0

class Camera {
  public:
    Transform transform = Transform::identity();
    mat4 getMatrix();

    Camera(float fov = 90.0f, float near = 0.1f,
           float far = 10000.0f);
    void inputs(vec2 bounds);

    vec3 interactionPoint;
    int clickState = NOCLICK;
    

  private:
    //
    // fov in degrees
    GLFWwindow* win_;
    float fov_, near_, far_;
    float sensitivity = 3.0f;
    float speed = 0.3f;
    float lockAxisX_ = glm::quarter_pi<float>();

    float rotX_ = 0;
    float rotY_ = 0;
};
