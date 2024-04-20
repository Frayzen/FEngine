#include "object.hh"
#include "mesh/mesh.hh"
#include "object/camera.hh"
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <GL/glew.h>
Object::Object(Mesh& m, unsigned int id) : m_(m), id_(id) {}

Transform Object::getTransform() {
    vec3 pos, skew, scale;
    vec4 persp;
    quat rot;
    glm::decompose(m_.getTransform(id_), scale, rot, pos, skew, persp);

    return Transform(pos, rot, scale);
}

void Object::setTransform(Transform &t) {
    m_.getTransform(id_) = t.getMatrix();
}
