#include "transform.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

mat4 Transform::getMatrix() {
    glm::mat4 m = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rot = getRotationMatrix();
    m = glm::scale(rot * m, scale);
    return m;
}

mat4 Transform::getRotationMatrix() {
    mat4 m = mat4(1.0f);
    m = rotate(m, rotation[0], vec3(1.0f, 0.0f, 0.0f));
    m = rotate(m, rotation[1], vec3(0.0f, 1.0f, 0.0f));
    m = rotate(m, rotation[2], vec3(0.0f, 0.0f, 1.0f));
    return m;
}

Transform Transform::identity() {
    Transform tf = Transform();
    tf.scale = vec3(1.0f, 1.0f, 1.0f);
    tf.position = vec3(0.0f, 0.0f, 0.0f);
    tf.rotation = vec3(0.0f, 0.0f, 0.0f);
    return tf;
}

vec3 Transform::front() {
    return glm::normalize(
        vec3(glm::inverse(getRotationMatrix()) * vec4(0.0f, 0.0f, 1.0f, 1.0f)));
}
vec3 Transform::left() {
    return glm::normalize(
        vec3(glm::inverse(getRotationMatrix()) * vec4(1.0f, 0.0f, 0.0f, 1.0f)));
}
vec3 Transform::up() {
    return glm::normalize(
        vec3(glm::inverse(getRotationMatrix()) * vec4(0.0f, 1.0f, 0.0f, 1.0f)));
}

Transform::Transform() {}
