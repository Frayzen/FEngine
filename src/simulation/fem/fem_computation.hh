#pragma once

#include "simulation/fem/fem_2d_mesh.hh"
#include <glm/fwd.hpp>
#include <vector>

using namespace glm;

std::vector<vec2> compute_displacement(std::vector<FEMPoint> points, std::vector<uvec2> elems);
