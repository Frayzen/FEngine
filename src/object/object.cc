#include "object.hh"
#include "object/camera.hh"
#include <glm/fwd.hpp>
#include <iostream>
#include <GL/glew.h>

Object::Object(Mesh &mesh) : attachedMesh_(mesh) { mesh.updateBuffers(); }

void Object::render(Shader &shader, Camera &camera) {
    shader.activate();

    GLuint tranformMatUniID = glGetUniformLocation(shader.getProgram(), "transformMat");
    glm::mat4 transformMat =  camera.getMatrix() * transform.getMatrix() ;
    glUniformMatrix4fv(tranformMatUniID, 1, GL_FALSE, &transformMat[0][0]);

    attachedMesh_.updateBuffers();
    attachedMesh_.enable();
    glDrawElements(GL_TRIANGLES, attachedMesh_.triangleNumber() * 3,
                   GL_UNSIGNED_INT, nullptr);
}
