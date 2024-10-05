#include "submesh.hh"
#include "object/object.hh"
#include "shader/render.hh"
#include "mesh.hh"
#include <assimp/mesh.h>
#include <glm/ext/vector_uint3.hpp>
#include <iostream>

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_UV 2
#define LOC_TRANSFORM 3
/* #define LOC_NEXT 8 */

SubMesh::SubMesh(Mesh &m) : mesh_(m), box_(AABBox::noBounds()) {
    VAO = VBO = EBO = TBO = materialId_ = 0;
}

SubMesh SubMesh::createFrom(Mesh &m, aiMesh *mesh) {
    SubMesh sm = SubMesh(m);
    if (!mesh->mNormals)
        std::cout << "/!\\ NO NORMALS FOUND IN " << mesh->mName.C_Str() << '\n';
    // Retrieve vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto v = mesh->mVertices[i];
        vec3 vert = {v.x, v.y, v.z};
        auto norm = mesh->mNormals[i];
        vec3 vn = {norm.x, norm.y, norm.z};
        vec3 uv = vec3(0);
        if (mesh->HasTextureCoords(0)) {
            auto uv_vec = mesh->mTextureCoords[0][i];
            uv = {uv_vec.x, uv_vec.y, uv_vec.z};
        }
        sm.vertices_.emplace_back(vert);
        sm.vertices_.emplace_back(vn);
        sm.vertices_.emplace_back(uv);
    }
    sm.materialId_ = mesh->mMaterialIndex;
    // Retrieve indices (assuming triangles)
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices == 3) // Assuming triangles
            sm.indices_.emplace_back(
                uvec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
    }
    std::cout << "[+] " << mesh->mName.C_Str() << " (" << mesh->mNumVertices
              << " vertices)" << '\n';
    return sm;
}

void SubMesh::updateBuffers() {
    if (VAO + VBO + EBO != 0)
        return;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vec3),
                 vertices_.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uvec3),
                 indices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 3,
                          nullptr);
    glEnableVertexAttribArray(LOC_VERTEX);
    glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 3,
                          (void *)sizeof(vec3));
    glEnableVertexAttribArray(LOC_NORMAL);
    glVertexAttribPointer(LOC_UV, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 3,
                          (void *)(2 * sizeof(vec3)));
    glEnableVertexAttribArray(LOC_UV);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SubMesh::~SubMesh() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &TBO);
    glDeleteVertexArrays(1, &VAO);
}

void SubMesh::enable() { glBindVertexArray(VAO); }

unsigned int SubMesh::triangleNumber() { return indices_.size(); }

void SubMesh::updateObjects() {
    glBindVertexArray(VAO);
    if (!TBO) {
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * mesh_.getObjects().size(),
                     Object::getTransforms(mesh_), GL_STATIC_DRAW);
        // Since vertexAttribPointer size maximum is 4
        for (int i = 0; i < 4; i++) {
            glVertexAttribPointer(LOC_TRANSFORM + i, 4, GL_FLOAT, GL_FALSE,
                                  sizeof(mat4), (void *)(sizeof(vec4) * i));
            glEnableVertexAttribArray(LOC_TRANSFORM + i);
            glVertexAttribDivisor(LOC_TRANSFORM + i, 1);
        }
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * mesh_.getObjects().size(),
                     Object::getTransforms(mesh_), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SubMesh::render(Render &r) {
    static Material defaultMaterial = Material();
    updateBuffers();
    updateObjects();
    if (mesh_.getMaterials().size() > materialId_)
        mesh_.getMaterials()[materialId_].enable(r);
    else
        defaultMaterial.enable(r);

    enable();
    glDrawElementsInstanced(GL_TRIANGLES, triangleNumber() * 3, GL_UNSIGNED_INT,
                            nullptr, mesh_.getObjects().size());
}

int SubMesh::addVertex(const vec3 &v, const vec3 &n, const vec3 &uv) {
    box_.update(v);
    vertices_.emplace_back(v);
    vertices_.emplace_back(n);
    vertices_.emplace_back(uv);
    return ((vertices_.size()) / 3) - 1;
}

int SubMesh::addTriangle(const uvec3 &v) {
    indices_.push_back(v);
    return indices_.size() - 1;
}

AABBox &SubMesh::getAABbox() { return box_; }

mat3 SubMesh::getTriangle(unsigned int id) {
    uvec3 vid = indices_[id];
    mat3 r;
    r[0] = vertices_[vid[0] * 3];
    r[1] = vertices_[vid[1] * 3];
    r[2] = vertices_[vid[2] * 3];
    return r;
}
