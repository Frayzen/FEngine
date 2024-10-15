#pragma once

#include "object/object.hh"
#include "shader/render.hh"
#include <assimp/mesh.h>
#include <glm/fwd.hpp>
#include "object/aabbox.hh"
#include <GL/gl.h>
#include <vector>
using namespace glm;

class SubMesh {
    friend Mesh;

  public:
    SubMesh(Mesh &m);
    static SubMesh createFrom(Mesh &m, aiMesh *mesh);
    void enable();
    mat3 getTriangle(unsigned int id);
    unsigned int triangleNumber();
    void render(Render &r);
    ~SubMesh();

    AABBox &getAABbox();

    int addVertex(const vec3 &v, const vec3 &n, const vec3 &uv);
    int addTriangle(const uvec3 &v);

  private:
    void updateBuffers();
    void updateObjects();

    Mesh &mesh_;

    unsigned int materialId_;

    // Linked to the VBO and contiaining the vertex data
    // For each vertex we have 3 vec3 (position, normal, uv) in this array
    std::vector<vec3> vertices_;
    std::vector<uvec3> indices_;
    // TBO = Transform Buffer Object
    // CBO = Color Buffer Object
    GLuint VAO, VBO, EBO, TBO;
    AABBox box_;
};
