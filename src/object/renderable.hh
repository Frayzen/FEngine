#pragma once

#include "GL/glew.h"

class Renderable {
  public:
    virtual void render(GLuint program) = 0;
};
