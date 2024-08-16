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

mat4 Camera::getProjMat() {

    const float ratio = screenSize.x / screenSize.y;
    return glm::perspective(glm::radians(fov_), ratio, near_, far_);
}

mat4 Camera::getViewMat() {
    return glm::lookAt(position, position + getFront(), up_);
}

vec3 Camera::getFront() {
    vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front = normalize(front);
    return front;
}

vec3 Camera::getRight() { return glm::cross(getFront(), up_); }

Camera::Camera(float fov, float near, float far)
    : fov_(fov), near_(near), far_(far) {
    assert(glfwGetCurrentContext() != nullptr);
    win_ = glfwGetCurrentContext();
}

void Camera::inputs() {
    if (glfwGetKey(win_, GLFW_KEY_D) == GLFW_PRESS)
        position += speed * getRight();
    if (glfwGetKey(win_, GLFW_KEY_A) == GLFW_PRESS)
        position += speed * -getRight();
    if (glfwGetKey(win_, GLFW_KEY_W) == GLFW_PRESS)
        position += speed * getFront();
    if (glfwGetKey(win_, GLFW_KEY_S) == GLFW_PRESS)
        position += speed * -getFront();
    if (glfwGetKey(win_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        position += speed * -up_;
    if (glfwGetKey(win_, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += speed * up_;

    if (glfwGetKey(win_, GLFW_KEY_L) == GLFW_PRESS)
        yaw_ += sensitivity;
    if (glfwGetKey(win_, GLFW_KEY_H) == GLFW_PRESS)
        yaw_ -= sensitivity;
    if (glfwGetKey(win_, GLFW_KEY_J) == GLFW_PRESS)
        pitch_ -= sensitivity;
    if (glfwGetKey(win_, GLFW_KEY_K) == GLFW_PRESS)
        pitch_ += sensitivity;
    pitch_ = max(min(pitch_, lockAxisX_), -lockAxisX_);
}

void Camera::mouseInput(vec2 bounds) {
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
