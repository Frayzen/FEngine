#pragma once

#include "shader/render.hh"
#include <assimp/material.h>
#include <GL/gl.h>
class Material {

  public:
    static Material createFrom(std::string folderRoot, aiMaterial *mat);
    void enable(Render& r);

  private:
    GLuint diffuseText_, specularText_ = 0;
    float shininess_, shininessStrength_ = 0.0f;
};
