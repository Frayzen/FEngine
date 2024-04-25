#pragma once

#include "GL/glew.h"
#include "object/camera.hh"
#include "shader/render.hh"
#include "shader/shader.hh"

class Renderable {
  public:
    virtual void render(Render& shader, Camera &camera) = 0;
};
