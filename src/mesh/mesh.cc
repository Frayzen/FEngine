#include <GL/glew.h>
#include "mesh.hh"
#include "mesh/submesh.hh"
#include "shader/render.hh"
#include "tools.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <cstdlib>
#include <cstring>

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
    std::cout << " = Creating the mesh..." << '\n';

    for (unsigned int mid = 0; mid < scene->mNumMeshes; mid++) {
        aiMesh *mesh = scene->mMeshes[mid];
        SubMesh &sm = m.subMeshes_.emplace_back(SubMesh(m));
        if (!mesh->mNormals)
            std::cout << "/!\\ NO NORMALS FOUND IN " << mesh->mName.C_Str()
                      << '\n';
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
                sm.indices_.emplace_back(uvec3(
                    face.mIndices[0], face.mIndices[1], face.mIndices[2]));
        }
        std::cout << "[+] " << mesh->mName.C_Str() << " (" << mesh->mNumVertices
                  << " vertices)" << '\n';
    }
    std::cout << " = Mesh built ! (" << m.subMeshes_.size() << " part.s)" << '\n';
    return m;
}

void Mesh::render(Render &shader, Camera &camera) {
    shader.activate();
    mat4 m = camera.getMatrix();
    for (SubMesh &sm : subMeshes_)
        sm.render(shader, m);
}

std::vector<Object> &Mesh::getObjects() { return objects_; }

Object &Mesh::createObject() { return objects_.emplace_back(Object(*this)); }
