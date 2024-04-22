#pragma once

#include "GL/glew.h"
#include "object/camera.hh"
#include "shader/shader.hh"

class Renderable {
  public:
    virtual void render(Shader& shader, Camera &camera) = 0;
};
