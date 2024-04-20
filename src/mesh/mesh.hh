#pragma once

#include "object/object.hh"
#include <GL/gl.h>
#include "object/transform.hh"
#include <glm/fwd.hpp>
#include <string>
#include <vector>

using namespace glm;

class Mesh : public Renderable{
  public:
    static Mesh createFrom(std::string path);
    ~Mesh();
    void enable();
    unsigned int triangleNumber();
    void render(Shader &shader, Camera &camera);

    Object& createObject();
    std::vector<Object>& getObjects();
    mat4& getTransform(unsigned int id);

  private:
    Mesh();
    void updateBuffers();
    void updateTransforms();
    void appendVertices(const vec3 *vertices, unsigned long size);
    void appendIndices(const uvec3 *indices, size_t size);

    std::vector<vec3> vertices_;
    std::vector<uvec3> indices_;
    //object related
    std::vector<Object> objects_;
    std::vector<mat4> objTransforms_;
    // IBT = Instance Buffer Transform
    GLuint VAO, VBO, EBO, IBT = 0;
};
