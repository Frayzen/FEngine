#include "terrain.hh"

#include <array>
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

using namespace glm;

float interpolate(float from, float to, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    return (2 * t3 - 3 * t2 + 1) * from - (2 * t3 - 3 * t2) * to;
}

vec2 randomVec2() {
    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return normalize(vec2(x, y));
}

static vec2 samples[SAMPLES][SAMPLES];
float dotSample(vec2 p, vec2 c1) {
    vec2 toCorner = normalize(p - c1);
    int x = (int)c1.x;
    int y = (int)c1.y;
    vec2 sample = samples[x][y];
    return dot(toCorner, sample);
}

float getPerlinNoise(vec2 p) {
    vec2 xy = floor(p);
    vec2 XY = xy + vec2(1, 1);
    vec2 xY = xy + vec2(0, 1);
    vec2 Xy = xy + vec2(1, 0);
    float tx = p.x - xy.x;
    float ty = p.y - xy.y;
    float v1 = interpolate(dotSample(p, xy), dotSample(p, Xy), tx);
    float v2 = interpolate(dotSample(p, xY), dotSample(p, XY), tx);
    return interpolate(v1, v2, ty);
}

void generateNoise(float vals[CHUNK_SIZE][CHUNK_SIZE]) {
    for (int i = 0; i < SAMPLES; i++)
        for (int j = 0; j < SAMPLES; j++)
            samples[i][j] = randomVec2();
    for (int i = 0; i < CHUNK_SIZE; i++)
        for (int j = 0; j < CHUNK_SIZE; j++)
            vals[i][j] = getPerlinNoise(vec2(i / 8.0f, j / 8.0f));
}

Terrain Terrain::generate(int seed) {
    srand(seed);
    static float vals[CHUNK_SIZE][CHUNK_SIZE];
    (void) vals;
    return Terrain();
}
