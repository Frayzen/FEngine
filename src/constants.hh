#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace glm;
static const glm::vec2 screenSize = glm::vec2(2000, 1000);
static const vec3 bounds = vec3(20.0f, 10.0f, 0.0f);
static const vec2 size = uvec2(10, 11);
static const vec2 offset = vec2(1.0f, 1.0f);

#define MINF (std::numeric_limits<float>::min())
#define MAXF (std::numeric_limits<float>::max())
static const vec3 lightPos = vec3(10000, 0, 0);

static const float maxZoom = 50;
static const float minZoom = 3;
