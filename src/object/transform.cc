#include "transform.hh"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

mat4 Transform::getMatrix() {
    glm::mat4 m = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rot = getRotationMatrix();
    m = rot * m;

    /* for (int i = 0; i < 4; i++) */
    /* { */
    /*     for (int j = 0; j < 4; j++) */
    /*     { */
    /*         std::cout << m[i][j] << " "; */
    /*     } */
    /*     std::cout << "" << '\n'; */

    /* } */
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
Transform::Transform() {}
