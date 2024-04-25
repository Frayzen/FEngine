#include "submesh.hh"
#include "object/object.hh"
#include "shader/render.hh"
#include "mesh.hh"
#include <assimp/mesh.h>
#include <iostream>

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TRANSFORM 2
#define LOC_COLOR 7

SubMesh::SubMesh(Mesh &m) : mesh_(m) {}

SubMesh SubMesh::createFrom(Mesh &m, aiMesh *mesh) {
    unsigned int mat = mesh->mMaterialIndex;
    std::cout << mat << '\n';
    SubMesh sm = SubMesh(m);
    if (!mesh->mNormals)
        std::cout << "/!\\ NO NORMALS FOUND IN " << mesh->mName.C_Str() << '\n';
    // Retrieve vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto v = mesh->mVertices[i];
        vec3 vert = {v.x, v.y, v.z};
        auto norm = mesh->mNormals[i];
        vec3 vn = {norm.x, norm.y, norm.z};
        sm.vertices_.emplace_back(vert);
        sm.vertices_.emplace_back(vn);
    }

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

    glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2,
                          nullptr);
    glEnableVertexAttribArray(LOC_VERTEX);
    glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) * 2,
                          (void *)sizeof(vec3));
    glEnableVertexAttribArray(LOC_NORMAL);

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
    if (!TBO || !CBO) {
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
        // CBO
        glGenBuffers(1, &CBO);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh_.getObjects().size(),
                     Object::getColors(mesh_), GL_STATIC_DRAW);
        glVertexAttribPointer(LOC_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(vec4),
                              nullptr);
        glEnableVertexAttribArray(LOC_COLOR);
        glVertexAttribDivisor(LOC_COLOR, 1);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * mesh_.getObjects().size(),
                     Object::getTransforms(mesh_), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * mesh_.getObjects().size(),
                     Object::getColors(mesh_), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SubMesh::render(Render &r, mat4 &camMat) {
    GLuint camMatUniID = glGetUniformLocation(r.getProgram(), "camMat");
    glUniformMatrix4fv(camMatUniID, 1, GL_FALSE, &camMat[0][0]);

    GLuint posLightUniID = glGetUniformLocation(r.getProgram(), "lightPos");
    vec3 lightPos = vec3(0.0f, 1000.f, 0.0f);
    glUniform3fv(posLightUniID, 1, &lightPos[0]);

    updateBuffers();
    updateObjects();
    enable();
    glDrawElementsInstanced(GL_TRIANGLES, triangleNumber() * 3, GL_UNSIGNED_INT,
                            nullptr, mesh_.getObjects().size());
}
