#include "object.hh"
#include "object/camera.hh"
#include <iostream>
#include <GL/glew.h>

Object::Object(Mesh &mesh) : attachedMesh_(mesh) { mesh.updateBuffers(); }

void Object::render(Shader &shader) {
    GLuint camMatUniID = glGetUniformLocation(shader.getProgram(), "camMat");
    glm::mat4 camMat = mainCamera.getMatrix();
    glUniformMatrix4fv(camMatUniID, 1, GL_FALSE,
                       reinterpret_cast<GLfloat *>(&camMat));
    attachedMesh_.updateBuffers();
    attachedMesh_.enable();
    glDrawElements(GL_TRIANGLES, attachedMesh_.triangleNumber() * 3,
                   GL_UNSIGNED_INT, nullptr);
}
