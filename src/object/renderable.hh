#pragma once

#include "shader/render.hh"
#include "object/camera.hh"

class Renderable {
  public:
    virtual void render(Render &shader, Camera &camera) = 0;
    virtual ~Renderable() = default;
};
