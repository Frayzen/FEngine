#include <GL/glew.h>
#include "mesh.hh"
#include "tools.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <cstdlib>
#include <cstring>

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TRANSFORM 2
#define LOC_COLOR 7

static int curId = 0;

Mesh::Mesh() : id(curId++) {}

Mesh Mesh::createFrom(std::string path) {
    std::cout << "Importing " << path << "..." << '\n';
    static Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path.c_str(),
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
            aiProcess_GlobalScale |
            aiProcess_GenSmoothNormals /* or aiProcess_GenNormals */);
    FAIL_ON(scene == nullptr, "The mesh " << path
                                          << "could not be loaded.\nReason:"
                                          << importer.GetErrorString());
    Mesh m = Mesh();

    int curId = 0;
    for (unsigned int mid = 0; mid < scene->mNumMeshes; mid++) {
        aiMesh *mesh = scene->mMeshes[mid];
        if (!mesh->mNormals)
            std::cout << "NO NORMALS FOUND" << '\n';
        // Retrieve vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            auto v = mesh->mVertices[i];
            vec3 vert = {v.x, v.y, v.z};
            auto norm = mesh->mNormals[i];
            vec3 vn = {norm.x, norm.y, norm.z};
            m.vertices_.push_back(vert);
            m.vertices_.push_back(vn);
        }

        // Retrieve indices (assuming triangles)
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices == 3) // Assuming triangles
                m.indices_.push_back(uvec3(curId + face.mIndices[0],
                                           curId + face.mIndices[1],
                                           curId + face.mIndices[2]));
        }
        curId += mesh->mNumVertices;
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

Mesh::~Mesh() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &TBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::enable() { glBindVertexArray(VAO); }

unsigned int Mesh::triangleNumber() { return indices_.size(); }

Object &Mesh::createObject() { return objects_.emplace_back(Object(*this)); }

void Mesh::updateObjects() {
    glBindVertexArray(VAO);
    if (!TBO || !CBO) {
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * objects_.size(),
                     Object::getTransforms(*this), GL_STATIC_DRAW);
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * objects_.size(),
                     Object::getColors(*this), GL_STATIC_DRAW);
        glVertexAttribPointer(LOC_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(vec4),
                              nullptr);
        glEnableVertexAttribArray(LOC_COLOR);
        glVertexAttribDivisor(LOC_COLOR, 1);
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * objects_.size(),
                     Object::getTransforms(*this), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * objects_.size(),
                     Object::getColors(*this), GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::render(Shader &shader, Camera &camera) {
    shader.activate();

    GLuint camMatUniID = glGetUniformLocation(shader.getProgram(), "camMat");
    glUniformMatrix4fv(camMatUniID, 1, GL_FALSE, &camera.getMatrix()[0][0]);

    GLuint posLightUniID =
        glGetUniformLocation(shader.getProgram(), "lightPos");
    vec3 lightPos = vec3(0.0f, 1000.f, 0.0f);
    glUniform3fv(posLightUniID, 1, &lightPos[0]);

    updateBuffers();
    updateObjects();
    enable();
    glDrawElementsInstanced(GL_TRIANGLES, triangleNumber() * 3, GL_UNSIGNED_INT,
                            nullptr, objects_.size());
}

std::vector<Object> &Mesh::getObjects() { return objects_; }
