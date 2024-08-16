#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>

using namespace glm;

#define RCLICK 1
#define LCLICK -1
#define NOCLICK 0

class Camera {
  public:
    mat4 getViewMat();
    mat4 getProjMat();

    Camera(float fov = 90.0f, float near = 0.1f, float far = 1000000000.0f);
    void mouseInput(vec2 bounds);
    void inputs();

    vec3 interactionPoint;
    int clickState = NOCLICK;

    vec3 position = vec3(0.0f);

    float sensitivity = 4.0f;
    float speed = 0.3f;

  private:
    vec3 getFront();
    vec3 getRight();
    // fov in degrees
    GLFWwindow *win_;
    float fov_, near_, far_;

    float lockAxisX_ = 89.9;

    vec3 up_ = vec3(0.0f, 1.0f, 0.0f);
    // in degrees
    float pitch_ = 0;
    float yaw_ = 0;
};
