#include "transform.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(vec3 pos, quat rot, vec3 scale)
    : position(pos), rotation(rot), scale(scale) {}

mat4 Transform::getMatrix() {
    glm::mat4 pos = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rot = getRotationMatrix();
    glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);
    return pos * rot * scl;
}

mat4 Transform::getRotationMatrix() {
    return glm::mat4_cast(rotation);
}

Transform Transform::identity() {
    Transform tf = Transform();
    tf.scale = vec3(1.0f, 1.0f, 1.0f);
    tf.position = vec3(0.0f, 0.0f, 0.0f);
    tf.rotation = glm::quat(vec3(0.0f));
;
    return tf;
}

vec3 Transform::front() {
    return glm::normalize(
        vec3(glm::inverse(getRotationMatrix()) * vec4(0.0f, 0.0f, 0.5f, 0.5f)));
}
vec3 Transform::left() {
    return glm::normalize(
        vec3(glm::inverse(getRotationMatrix()) * vec4(0.5f, 0.0f, 0.0f, 0.5f)));
}
vec3 Transform::up() {
    return glm::normalize(
        vec3(glm::inverse(getRotationMatrix()) * vec4(0.0f, 0.5f, 0.0f, 0.5f)));
}

Transform::Transform() {}


void Transform::setRotation(vec3 v)
{
   rotation = quat(vec3(0,0,0)); 
   rotation = rotate(rotation, v.x, vec3(1.0f, 0.0f, 0.0f));
   rotation = rotate(rotation, v.y, vec3(0.0f, 1.0f, 0.0f));
   rotation = rotate(rotation, v.z, vec3(0.0f, 0.0f, 1.0f));
}
