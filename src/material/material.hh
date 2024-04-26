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

    float shininess = 0.0f;
    float shininessStrength = 0.0f;
    vec3 diffuseCol, specularCol = vec3(1.0f);
    vec3 ambientCol = vec3(1.0f);

  private:
    GLuint diffuseText_, specularText_ = 0;
};
