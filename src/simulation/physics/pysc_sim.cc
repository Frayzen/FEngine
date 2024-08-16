#include "pysc_sim.hh"
#include "object/transform.hh"
#include <glm/fwd.hpp>

void PyscSimulation::init() {
    Object &o = sphere_.createObject();
    auto t = o.getTransform();
    t.scale = vec3(10.0f);
    o.setTransform(t);
}

void PyscSimulation::update(double deltaTime) { (void)deltaTime; }

PyscSimulation::PyscSimulation()
    : Simulation(), sphere_(Mesh::generateSphere(100, 100)) {
    registerMesh(sphere_);
    bgColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
