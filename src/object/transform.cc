#include "transform.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

mat4 Transform::getMatrix() {
    mat4 m = mat4(1.0f);
    // position
    m = translate(m, position);
    // rotate
    m = rotate(m, rotation[0], vec3(1.0f, 0.0f, 0.0f));
    m = rotate(m, rotation[1], vec3(0.0f, 1.0f, 0.0f));
    m = rotate(m, rotation[2], vec3(0.0f, 0.0f, 1.0f));
    // scale
    return m;
}

Transform Transform::identity() {
    Transform tf = Transform();
    tf.scale = vec3(1.0f, 1.0f, 1.0f);
    tf.position = vec3(0.0f, 0.0f, 0.0f);
    tf.rotation = vec3(0.0f, 0.0f, 0.0f);
    return tf;
}
Transform::Transform() {}
