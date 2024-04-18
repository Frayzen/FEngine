#include <GL/glew.h>
#include "mesh.hh"
#include "tools.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <cstdlib>
#include <cstring>

Mesh::Mesh() {}

Mesh Mesh::createFrom(std::string path) {
    std::cout << "Importing " << path << "..." << '\n';
    static Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
    FAIL_ON(scene == nullptr, "The mesh " << path
                                          << "could not be loaded.\nReason:"
                                          << importer.GetErrorString());
    Mesh m = Mesh();

    aiMesh *mesh =
        scene->mMeshes[0]; // Assuming there's only one mesh in the scene

    // Retrieve vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto b = mesh->mVertices[i];
        vec3 v = {b.x, b.y, b.z};
        v = v * 0.1f;
        m.vertices_.push_back(v);
    }

    // Retrieve indices (assuming triangles)
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        if (face.mNumIndices == 3) // Assuming triangles
            m.indices_.push_back(
                uvec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
    }

    return m;
}

void Mesh::appendVertices(const vec3 *vertices, size_t size) {
    vertices_.insert(vertices_.end(), vertices, vertices + size);
}

void Mesh::appendIndices(const uvec3 *indices, size_t size) {
    indices_.insert(indices_.end(), indices, indices + size);
}

void Mesh::updateBuffers() {
    if (VAO + VBO + EBO != 0)
        return;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    enable();

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vec3),
                 vertices_.data(), GL_STATIC_DRAW);
    FAIL_ON(indices_.size() % 3 != 0,
            "Invalid number of indices (got " << indices_.size() << ")")

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uvec3),
                 indices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::enable() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

unsigned int Mesh::triangleNumber() { return indices_.size(); }
