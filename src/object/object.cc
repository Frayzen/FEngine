#include "object.hh"
#include <iostream>

Object::Object(Mesh &mesh) : attachedMesh_(mesh) { mesh.updateBuffers(); }

void Object::render(GLuint program) {
    attachedMesh_.updateBuffers();
    glUseProgram(program);
    attachedMesh_.enable();

    glDrawElements(GL_TRIANGLES, attachedMesh_.triangleNumber(),
                   GL_UNSIGNED_INT, nullptr);
}
