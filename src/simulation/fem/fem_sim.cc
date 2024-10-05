#include "fem_sim.hh"
#include "simulation/simulation.hh"
#include <glm/fwd.hpp>
#include <iostream>

#define GAP_RECT 0.05
#define SQR_SIZE (1 - GAP_RECT / 2)

FemSimulation::FemSimulation()
    : Simulation(true), tile_(Mesh::generate2DRect(SQR_SIZE, SQR_SIZE)),
      beam_(Mesh::generate2DRect(1, .2)) {
    tile_.getMaterials()[0].setColor(vec3(1.0f, 1.0f, 1.0f));
    bgColor = vec4(0.5, 0, 0, 0);
    cam.speed = 0.2;
    registerMesh(beam_);
    registerMesh(tile_);
}

void FemSimulation::update(double deltaTime) {
    (void)deltaTime;
    /* auto p = cam.position; */
    /* std::cout << p.x << " " << p.y << " " << p.x << std::endl; */
    /* p = tile_.getObjects()[0].getTransform().position; */
    /* std::cout << p.x << " " << p.y << " " << p.x << std::endl; */
}
void FemSimulation::init() {
    for (int i = -100; i < 100; i++) {
        for (int j = -100; j < 100; j++) {
            auto &o = tile_.createObject();
            auto t = o.getTransform();
            t.position = vec3(0, i + GAP_RECT / 2, j + GAP_RECT / 2);
            o.setTransform(t);
        }
    }
}
