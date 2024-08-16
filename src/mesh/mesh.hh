#pragma once

#include "object/renderable.hh"
#include "object/object.hh"
#include <GL/gl.h>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

using namespace glm;

class Mesh : public Renderable {
  public:
    static Mesh generateSphere(int slices, int stacks);
    static Mesh createFrom(std::string path);
    ~Mesh();
    void enable();
    unsigned int triangleNumber();
    void render(Shader &shader, Camera &camera) override;

    Object &createObject();
    void clearObjects();
    std::vector<Object> &getObjects();
    const unsigned int id;

  private:
    Mesh();
    void updateBuffers();
    void updateObjects();
    void appendVertices(const vec3 *vertices, unsigned long size);
    int addVertex(const vec3 &v);
    void appendIndices(const uvec3 *indices, size_t size);
    int addTriangle(const uvec3 &v);

    std::vector<vec3> vertices_;
    std::vector<uvec3> indices_;
    // object related
    std::vector<Object> objects_;
    // TBO = Transform Buffer Object
    // CBO = Color Buffer Object
    GLuint VAO, VBO, EBO, TBO, CBO = 0;
};
