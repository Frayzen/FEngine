#pragma once

#include <glm/fwd.hpp>

#define CHUNK_SIZE 128
#define SAMPLES 8

class Terrain
{
   static Terrain generate(int seed);
};
