#include "fem_2d_mesh.hh"
#include "simulation/fem/fem_computation.hh"
#include "simulation/fem/fem_consts.hh"
#include "simulation/simulation.hh"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

FEM2DMesh::FEM2DMesh()
    : beam_(Mesh::generate2DRect(1, GAP_RECT * 3)),
      rollerXIndicator_(Mesh::generate2DRect(0.2f, 1)),
      rollerYIndicator_(Mesh::generate2DRect(1, 0.2f)),
      fixedIndicator_(Mesh::generateSphere(10, 10, 0.2f)) {
    beam_.getMaterials()[0].setColor(vec3(0.0, 0.0, 1.0), true);
    rollerXIndicator_.getMaterials()[0].setColor(vec3(0.0f, 1.0f, 0.0f), true);
    rollerYIndicator_.getMaterials()[0].setColor(vec3(0.0f, 1.0f, 0.0f), true);
    fixedIndicator_.getMaterials()[0].setColor(vec3(1.0f, 0.0f, 0.0f), true);
}

bool FEM2DMesh::is_well_defined()
{
  bool oneFixed = false;
  vec3 fixedpos;
  for (auto& p : elems0d_)
    if (p.flags == FIXED)
    {
      oneFixed = true;
      fixedpos = p.coord;
      break;
    }
  if (!oneFixed)
    return false;
  vec3 dir = vec3(0);
  for (auto& p : elems0d_)
  {
    if (p.flags == NONE)
      continue;
    if (dir.x == 0 && dir.y == 0)
      dir = fixedpos - p.coord;
    else if(glm::length(cross(dir, fixedpos - p.coord)) <= 0.01f)
      continue;
    return true;
  }
  return false;
}

int find_or_append(vec3 e, std::vector<FEMPoint> &v) {
    for (unsigned int i = 0; i < v.size(); i++) {
        if (v[i].coord == e)
            return i;
    }
    auto newpt = FEMPoint{e, vec3(0), NONE, vec3(0)};
    v.push_back(newpt);
    return v.size() - 1;
}

void FEM2DMesh::add_beam(vec3 v1, vec3 v2) {
    auto o = beam_.createObject();
    o.setScale(vec3(SQR_SIZE, SQR_SIZE, 0));

    int id1 = find_or_append(v1, elems0d_);
    int id2 = find_or_append(v2, elems0d_);
    if (std::find(elems1d_.begin(), elems1d_.end(), uvec2(id1, id2)) !=
        elems1d_.end())
        return;

    elems1d_.push_back(uvec2(id1, id2));
    updatePos(elems1d_.size() - 1);
}

void FEM2DMesh::updatePos(int id) {
    Object &o = beam_.getObjects()[id];
    vec3 v1 =
        elems0d_[elems1d_[id].x].coord + elems0d_[elems1d_[id].x].displacement;
    vec3 v2 =
        elems0d_[elems1d_[id].y].coord + elems0d_[elems1d_[id].y].displacement;

    auto dir = v1 - v2;
    auto len = length(dir);
    auto rot = glm::quat_cast(
        glm::rotate(identity<mat4>(), std::atan2(dir.z, dir.y), vec3(1, 0, 0)));
    Transform t = o.getTransform();
    t.scale = vec3(len, len, 1);
    t.position = (v1 + v2) / 2.0f;
    t.rotation = rot;
    o.setTransform(t);
    updateFlags();
}

void FEM2DMesh::compute(void) {
    compute_displacement(elems0d_, elems1d_);
    for (unsigned int i = 0; i < elems1d_.size(); i++)
        updatePos(i);
}

void FEM2DMesh::registerMeshes(Simulation &sim) {
    sim.registerMesh(fixedIndicator_);
    sim.registerMesh(rollerXIndicator_);
    sim.registerMesh(rollerYIndicator_);
    sim.registerMesh(beam_);
}

void FEM2DMesh::reset() {
    elems0d_.clear();
    elems1d_.clear();
    updateFlags();
}

void FEM2DMesh::updateFlags() {
    rollerYIndicator_.clearObjects();
    rollerXIndicator_.clearObjects();
    fixedIndicator_.clearObjects();
    for (auto e : elems0d_) {
        auto pos = e.coord + e.displacement;
        switch (e.flags) {
        case FIXED:
            fixedIndicator_.createObject().setPosition(pos);
            break;
        case ROLLING_X:
            rollerXIndicator_.createObject().setPosition(pos);
            break;
        case ROLLING_Y:
            rollerYIndicator_.createObject().setPosition(pos);
            break;
        default:
            break;
        }
    }
}

bool FEM2DMesh::setMode(vec3 pt, FEMFlag flag) {

    FEMPoint *p = getPoint(pt);
    if (!p)
        return false;
    if (p->flags == flag)
        p->flags = NONE;
    else
        p->flags = flag;
    updateFlags();
    return true;
}

FEMPoint *FEM2DMesh::getPoint(vec3 pt) {
    unsigned int i = 0;
    for (; i < elems0d_.size(); i++) {
        if (elems0d_[i].coord == pt)
            break;
    }
    if (i == elems0d_.size())
        return NULL;
    return &elems0d_[i];
}

void FEM2DMesh::resetForces() {
    for (auto &p : elems0d_)
        p.forceApplied = vec3(0);
    compute();
}
