#include "transform.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Transform::Transform(vec3 pos, quat rot, vec3 scale)
    : position(pos), rotation(rot), scale(scale) {}

mat4 Transform::getMatrix() {
    glm::mat4 m = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rot = getRotationMatrix();
    m = glm::scale(rot * m, scale);
    return m;
}

mat4 Transform::getRotationMatrix() {
    return glm::mat4_cast(rotation);
}

Transform Transform::identity() {
    Transform tf = Transform();
    tf.scale = vec3(1.0f, 1.0f, 1.0f);
    tf.position = vec3(0.0f, 0.0f, 0.0f);
    tf.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
;
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
