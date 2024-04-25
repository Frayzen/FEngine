#pragma once

#include "shader/render.hh"
#include <glm/vec3.hpp>
#include <assimp/material.h>
#include <GL/gl.h>

using namespace glm;

class Material {

  public:
    static Material createFrom(std::string folderRoot, aiMaterial *mat);
    void enable(Render& r);

  private:
    GLuint diffuseText_, specularText_ = 0;
    float shininess_ = 0.0f;
    float shininessStrength_  = 1.0f;
    vec3 diffuseCol_, specularCol_, ambientCol_;
};
