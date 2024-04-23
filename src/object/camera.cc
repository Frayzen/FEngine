#include "camera.hh"
#include "constants.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <cassert>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

mat4 Camera::getMatrix() {
    mat4 m = transform.getMatrix();
    mat4 pers = glm::perspective(glm::radians(fov_),
                                 screenSize.x / screenSize.y, near_, far_);
    m = pers * m;
    return m;
}

Camera::Camera(float fov, float near, float far)
    : fov_(fov), near_(near), far_(far) {
    assert(glfwGetCurrentContext() != nullptr);
    win_ = glfwGetCurrentContext();
}

void Camera::inputs() {
    transform.setRotation(vec3(0, rotY_, 0));
    if (glfwGetKey(win_, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += speed * -transform.left();
    if (glfwGetKey(win_, GLFW_KEY_A) == GLFW_PRESS)
        transform.position += speed * transform.left();
    if (glfwGetKey(win_, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += speed * transform.front();
    if (glfwGetKey(win_, GLFW_KEY_S) == GLFW_PRESS)
        transform.position += speed * -transform.front();
    if (glfwGetKey(win_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        transform.position += speed * transform.up();
    if (glfwGetKey(win_, GLFW_KEY_SPACE) == GLFW_PRESS)
        transform.position += speed * -transform.up();

    float deg = glm::radians(sensitivity);
    if (glfwGetKey(win_, GLFW_KEY_L) == GLFW_PRESS)
        rotY_ += deg;
    if (glfwGetKey(win_, GLFW_KEY_H) == GLFW_PRESS)
        rotY_ -= deg;
    if (glfwGetKey(win_, GLFW_KEY_J) == GLFW_PRESS)
        rotX_ += deg;
    if (glfwGetKey(win_, GLFW_KEY_K) == GLFW_PRESS)
        rotX_ -= deg;
    rotX_ = max(min(rotX_, lockAxisX_), -lockAxisX_);
    transform.setRotation(vec3(rotX_, rotY_, 0));

    double xpos, ypos;
    int height, width;
    glfwGetCursorPos(win_, &xpos, &ypos);
    glfwGetWindowSize(win_, &width, &height);
    float x = 2 * bounds.x * (-0.5 + xpos / width);
    float y = 2 * bounds.y * (0.5 - ypos / height);
    interactionPoint = vec3(x, y, 0.0f);
    clickState = NOCLICK;
    if (glfwGetMouseButton(win_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        clickState += LCLICK;
    if (glfwGetMouseButton(win_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        clickState += RCLICK;
}

Camera &Camera::mainCamera() {
    static Camera camera;
    return camera;
}
