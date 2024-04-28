#include "terrain.hh"

#include <array>
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

using namespace glm;
#include <math.h>

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float interpolate(float a0, float a1, float w) {
    if (0.0 > w)
        return a0;
    if (1.0 < w)
        return a1;
    /* return (a1 - a0) * w + a0; */
    /* return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0; */
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

/* Create pseudorandom direction vector
 */
vec2 randomGradient() {
    // No precomputed gradients mean this works for any number of grid
    // coordinates
    float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return normalize(vec2(x, y));
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vec2 gradient = randomGradient();

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    std::cout << dx << " " << dy << '\n';

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return (0.5 * value) + 0.5; // Will return in range 0 to 1.
}
Terrain Terrain::generate() {
    static unsigned char img[CHUNK_SIZE][CHUNK_SIZE] = {{125}};
    for (int s = 0; s < 6; s++) {
        const float ratio = s / (float)CHUNK_SIZE;
        for (int i = 0; i < CHUNK_SIZE; i++)
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                img[i][j] /= 2;
                img[i][j] += perlin(i * ratio, j * ratio) * 255 / 2;
            }
    }
    const int channels = 1;
    stbi_write_png("perlin.png", CHUNK_SIZE, CHUNK_SIZE, channels, img,
                   CHUNK_SIZE * channels);
    return Terrain();
}
