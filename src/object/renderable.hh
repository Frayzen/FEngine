#pragma once

#include "GL/glew.h"
#include "shader.hh"

class Renderable {
  public:
    virtual void render(Shader& shader) = 0;
};
