#include "fem_2d_mesh.hh"
#include "simulation/fem/fem_computation.hh"
#include "simulation/fem/fem_consts.hh"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <vector>

FEM2DMesh::FEM2DMesh() : beam_(Mesh::generate2DRect(1, GAP_RECT * 3)) {
    beam_.getMaterials()[0].setColor(vec3(0.0, 0.0, 1.0), true);
}

int find_or_append(vec3 e, std::vector<FEMPoint> &v) {
    for (unsigned int i = 0; i < v.size(); i++) {
        if (v[i].coord == e)
            return i;
    }
    auto newpt = FEMPoint{e, NONE, vec3(0)};
    v.push_back(newpt);
    return v.size() - 1;
}

void FEM2DMesh::add_beam(vec3 v1, vec3 v2) {
    auto o = beam_.createObject();
    o.setScale(vec3(SQR_SIZE, SQR_SIZE, 0));

    int id1 = find_or_append(v1, elems0d_);
    int id2 = find_or_append(v2, elems0d_);

    elems1d_.push_back(uvec2(id1, id2));
    updatePos(elems1d_.size() - 1);
}

void FEM2DMesh::updatePos(int id) {
    Object &o = beam_.getObjects()[id];
    vec3 v1 = elems0d_[elems1d_[id].x].coord;
    vec3 v2 = elems0d_[elems1d_[id].y].coord;

    auto dir = v1 - v2;
    auto len = length(dir);
    auto rot = glm::quat_cast(
        glm::rotate(identity<mat4>(), std::atan2(dir.z, dir.y), vec3(1, 0, 0)));
    Transform t = o.getTransform();
    t.scale = vec3(len, len, 1);
    t.position = (v1 + v2) / 2.0f;
    t.rotation = rot;
    o.setTransform(t);
}

void FEM2DMesh::compute(void)
{
  auto d = compute_displacement(elems0d_, elems1d_);
  std::cout << "DISPLACEMENT:" << std::endl;
  for (unsigned int i = 0; i < d.size(); i++)
    std::cout << d[i].x << " " << d[i].y << std::endl;
}

Mesh &FEM2DMesh::getMesh(void) { return beam_; }

void FEM2DMesh::reset() {
  elems0d_.clear();
  elems1d_.clear();
}
