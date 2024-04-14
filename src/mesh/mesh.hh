#pragma once

#include <GL/gl.h>
#include "object/transform.hh"
#include <glm/fwd.hpp>
#include <string>
#include <vector>

using namespace glm;

class Mesh {
  public:
    static Mesh createFrom(std::string path);
    ~Mesh();
    void enable();
    void updateBuffers();
    unsigned int triangleNumber();

  private:
    Mesh();
    void appendVertices(const vec3 *vertices, unsigned long size);
    void appendIndices(const uvec3 *indices, size_t size);

    std::vector<vec3> vertices_;
    std::vector<uvec3> indices_;
    GLuint VAO, VBO, EBO = 0;
};
