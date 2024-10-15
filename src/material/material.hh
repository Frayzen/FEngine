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
    void setColor(vec3 c, bool noDiffuse = false);
    bool twosided = false;
    float shininess, opacity = 1.0f;
    float shininessStrength = 1.0f;
    vec3 diffuseCol = vec3(0.5f), specularCol = vec3(0.0f), ambientCol = vec3(0.5f);
    vec3 emissiveCol = vec3(0.0f);


  private:
    int textureMask_ = 0;
    GLuint diffuseText_, specularText_, ambientText_ = 0;
    GLuint normalMap_ = 0;
};

#define AMBIENT_TEXMASK (1 << 0)
#define DIFFUSE_TEXMASK (1 << 1)
#define SPECULAR_TEXMASK (1 << 2)
#define NORMAL_MAPMASK (1 << 3)
