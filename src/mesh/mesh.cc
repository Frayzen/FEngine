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
        path.c_str(), aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                          aiProcess_JoinIdenticalVertices |
                          aiProcess_SortByPType /* | aiProcess_MakeLeftHanded*/
    );
    FAIL_ON(scene == nullptr, "The mesh " << path
                                          << "could not be loaded.\nReason:"
                                          << importer.GetErrorString());
    Mesh m = Mesh();
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace& face = mesh->mFaces[j];
            m.appendIndices(reinterpret_cast<uvec3 *>(face.mIndices),
                            face.mNumIndices / 3);
        }
        m.appendVertices(reinterpret_cast<const vec3 *>(mesh->mVertices),
                         mesh->mNumVertices / 3);
    }
    std::cout << path << " succesfully imported" << '\n';
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

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vec3), vertices_.data(),
                 GL_STATIC_DRAW);
    FAIL_ON(indices_.size() % 3 != 0,
            "Invalid number of indicices (got " << indices_.size() << ")")

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uvec3), indices_.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
    glEnableVertexAttribArray(0);

    /* glBindVertexArray(0); */
    /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
    /* glBindBuffer(GL_ARRAY_BUFFER, 0); */
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
