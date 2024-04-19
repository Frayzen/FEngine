#include "object.hh"
#include "object/camera.hh"
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <GL/glew.h>
Object::Object(mat4 &t) : mat(t) {}
Transform Object::getTransform() {
    vec3 pos, skew, scale;
    vec4 persp;
    quat rot;
    glm::decompose(mat, scale, rot, pos, skew, persp);

    return Transform(pos, rot, scale);
}

void Object::setTransform(Transform &t) { mat = t.getMatrix(); }
