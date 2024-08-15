#pragma once

#include "shader/shader.hh"
#include "object/camera.hh"

class Renderable {
  public:
    virtual void render(Shader &shader, Camera &camera) = 0;
    virtual ~Renderable() = default;
};
