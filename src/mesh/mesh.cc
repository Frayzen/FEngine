#include <GL/glew.h>
#include "constants.hh"
#include "material/material.hh"
#include "mesh.hh"
#include "mesh/submesh.hh"
#include "shader/render.hh"
#include "tools.hh"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>

static int curId = 0;

Mesh::Mesh() : id(curId++) {}

Mesh Mesh::createFrom(std::string path) {
    std::cout << "Importing " << path << "..." << '\n';
    static Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path.c_str(),
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
            aiProcess_GenUVCoords | aiProcess_FlipUVs |
            aiProcess_RemoveRedundantMaterials |
            aiProcess_GenSmoothNormals /* or aiProcess_GenNormals */);
    FAIL_ON(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
                !scene->mRootNode,
            "The mesh " << path << "could not be loaded.\nReason:"
                        << importer.GetErrorString());
    auto p = std::filesystem::path(path);
    Mesh m = Mesh();
    std::cout << " = Creating the mesh..." << '\n';
    std::cout << scene->mNumMaterials << " materials detected" << '\n';
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
        m.materials_.emplace_back(
            Material::createFrom(p.parent_path(), scene->mMaterials[i]));
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        m.subMeshes_.emplace_back(SubMesh::createFrom(m, scene->mMeshes[i]));
    std::cout << " = Mesh built ! (" << m.subMeshes_.size() << " parts)"
              << '\n';
    return m;
}

void Mesh::render(Render &r, Camera &camera) {
    r.activate();
    r.setMat4("projection", camera.getProjMat());
    r.setMat4("view", camera.getViewMat());
    r.setVec3("lightPos", lightPos);
    r.setVec3("camPos", camera.position);
    for (SubMesh &sm : subMeshes_)
        sm.render(r);
}

std::vector<Object> &Mesh::getObjects() { return objects_; }
std::vector<Material> &Mesh::getMaterials() { return materials_; }

Object &Mesh::createObject() { return objects_.emplace_back(Object(*this)); }
