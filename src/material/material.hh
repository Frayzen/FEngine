#pragma once

#include "shader/render.hh"
#include <glm/vec3.hpp>
#include <assimp/material.h>
#include <GL/gl.h>

using namespace glm;

class Material {

  public:
    static Material createFrom(std::string folderRoot, aiMaterial *mat);
    void enable(Render &r);

    bool twosided = false;
    float shininess, opacity = 1.0f;
    float shininessStrength = 1.0f;
    vec3 diffuseCol, specularCol, ambientCol = vec3(-1.0f);
    vec3 emissiveCol = vec3(-1.0f);


  private:
    int textureMask_ = 0;
    GLuint diffuseText_, specularText_, ambientText_ = 0;
};

#define AMBIENT_TEXMASK (1 << 0)
#define AMBIENT_COLMASK (1 << 1)
#define DIFFUSE_TEXMASK (1 << 2)
#define DIFFUSE_COLMASK (1 << 3)
#define SPECULAR_TEXMASK (1 << 4)
#define SPECULAR_COLMASK (1 << 5)
