#include "camera.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>

mat4 Camera::getMatrix() {
    mat4 m = transform.getMatrix();
    /* mat4 pers = glm::perspective(fov_, WIDTH / (HEIGHT * 1.0f), near_, far_); */
    /* m *= pers; */
    /* std::cout << "MAT" << '\n'; */
    /* for (int i = 0; i < 4; i++) { */
    /*     for (int j = 0; j < 4; j++) { */
    /*         std::cout << m[i][j] << " "; */
    /*     } */
    /*     std::cout << "\n"; */
    /* } */
    return m;
}

Camera::Camera(float fov, float near, float far)
    : fov_(fov), near_(near), far_(far) {}

void Camera::inputs(GLFWwindow *win) {
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += speed * vec3(transform.getRotationMatrix() *
                                           vec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        transform.position += speed * vec3(transform.getRotationMatrix() *
                                           vec4(-1.0f, 0.0f, 0.0f, 1.0f));
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        transform.position += speed * vec3(transform.getRotationMatrix() *
                                           vec4(0.1f, 0.0f, 1.0f, 1.0f));
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += speed * vec3(transform.getRotationMatrix() *
                                           vec4(0.0f, 0.0f, -1.0f, 1.0f));
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
        transform.position += speed * vec3(transform.getRotationMatrix() *
                                           vec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        transform.position += speed * vec3(transform.getRotationMatrix() *
                                           vec4(0.0f, -1.0f, 0.0f, 1.0f));
}
Camera& Camera::mainCamera() {
    static Camera camera;
    return camera;
}
