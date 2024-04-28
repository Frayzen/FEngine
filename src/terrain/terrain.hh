#pragma once

#include <glm/fwd.hpp>

#define CHUNK_SIZE 128

class Terrain {
  public:
    static Terrain generate();
};
