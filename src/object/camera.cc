#include "camera.hh"
#include "tools.hh"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>

mat4 Camera::getMatrix() {
    mat4 m = transform.getMatrix();
    /* mat4 pers = glm::perspective(fov_, WIDTH / (HEIGHT * 1.0f), near_, far_); */
    /* m *= pers; */
    return m;
}

Camera::Camera(float fov, float near, float far)
    : fov_(fov), near_(near), far_(far) {}
