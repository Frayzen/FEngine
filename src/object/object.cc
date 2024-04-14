#include "object.hh"
#include "object/camera.hh"
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>

Object::Object(Mesh &mesh) : attachedMesh_(mesh) { mesh.updateBuffers(); }

void Object::render(Shader &shader, Camera &camera) {
    shader.activate();
    GLuint camMatUniID = glGetUniformLocation(shader.getProgram(), "camMat");
    glm::mat4 camMat = camera.getMatrix();
    glUniformMatrix4fv(camMatUniID, 1, GL_FALSE, &camMat[0][0]);
    attachedMesh_.updateBuffers();
    attachedMesh_.enable();
    glDrawElements(GL_TRIANGLES, attachedMesh_.triangleNumber() * 3,
                   GL_UNSIGNED_INT, nullptr);
}
