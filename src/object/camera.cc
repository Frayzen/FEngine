#include "camera.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

mat4 Camera::getMatrix() {
    mat4 m = transform.getMatrix();
    mat4 pers = glm::perspective(glm::radians(fov_), WIDTH / (1.0f * HEIGHT),
                                 near_, far_);
    m = pers * m;
    return m;
}

Camera::Camera(float fov, float near, float far)
    : fov_(fov), near_(near), far_(far) {}

void Camera::inputs(GLFWwindow *win) {
    transform.setRotation(vec3(0, rotY_, 0));
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += speed * -transform.left();
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        transform.position += speed * transform.left();
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += speed * transform.front();
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        transform.position += speed * -transform.front();
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        transform.position += speed * transform.up();
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
        transform.position += speed * -transform.up();

    float deg = glm::radians(sensitivity);
    if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS)
        rotY_ += deg;
    if (glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS)
        rotY_ -= deg;
    if (glfwGetKey(win, GLFW_KEY_J) == GLFW_PRESS)
        rotX_ += deg;
    if (glfwGetKey(win, GLFW_KEY_K) == GLFW_PRESS)
        rotX_ -= deg;
    rotX_ = max(min(rotX_, glm::half_pi<float>()), -glm::half_pi<float>());
    transform.setRotation(vec3(rotX_, rotY_, 0));
}
Camera &Camera::mainCamera() {
    static Camera camera;
    return camera;
}
