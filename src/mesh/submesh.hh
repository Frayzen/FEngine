#pragma once

#include "object/object.hh"
#include "shader/render.hh"
#include <assimp/mesh.h>
#include <glm/fwd.hpp>
#include <GL/gl.h>
#include <vector>
using namespace glm;

class SubMesh {
    friend Mesh;

  public:
    SubMesh(Mesh &m);
    static SubMesh createFrom(Mesh& m, aiMesh* mesh);
    void enable();
    unsigned int triangleNumber();
    void render(Render &r);
    ~SubMesh();

  private:
    void updateBuffers();
    void updateObjects();

    int addVertex(const vec3 &v);
    int addTriangle(const uvec3 &v);

    Mesh &mesh_;

    unsigned int materialId_ = 0;

    std::vector<vec3> vertices_;
    std::vector<uvec3> indices_;
    // TBO = Transform Buffer Object
    // CBO = Color Buffer Object
    GLuint VAO, VBO, EBO, TBO, CBO = 0;
};
