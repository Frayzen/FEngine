#include "fem_sim.hh"
#include "object/object.hh"
#include "object/ray.hh"
#include "simulation/simulation.hh"
#include <cmath>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

#define GAP_RECT 0.05
#define SQR_SIZE (1 - GAP_RECT / 2)

FemSimulation::FemSimulation()
    : Simulation(true), tile_(Mesh::generate2DRect(SQR_SIZE, SQR_SIZE)),
      beam_(Mesh::generate2DRect(1, .2)),
      selector_(Mesh::generateSphere(10, 10)) {
    tile_.getMaterials()[0].setColor(vec3(1.0f, 1.0f, 1.0f), true);
    selector_.getMaterials()[0].setColor(vec3(1.0f, 0.0f, 0.0f), true);
    bgColor = vec4(0, 0, 0, 0);
    cam.speed = 0.2;
    registerMesh(selector_);
    registerMesh(beam_);
    registerMesh(tile_);
}

void FemSimulation::update(double deltaTime) {
    (void)deltaTime;
    auto pt = cam.mouseInput(vec2(1, 1));
    auto interact = vec4(pt.x, pt.y, 0, 1);
    auto proj = inverse(cam.getProjMat());
    auto worldPos = proj * interact;
    worldPos /= worldPos.w;
    auto wp = vec3(worldPos);

    auto pos = cam.position;
    pos.y += wp.y;
    pos.z += wp.x;

    Ray r(pos, cam.getFront());

    int id;
    vec3 res;
    if (r.intersect(tile_.getObjects(), id, res)) {
        res.x = 0;
        res.y = round(res.y);
        res.z = round(res.z);
        selector_.getObjects()[0].setPosition(res);
    }
}
#define TILE_WORLD_SIZE 10
void FemSimulation::init() {
    cam.position = vec3(-5, 0, 0);
    Object &cursor = selector_.createObject();
    cursor.setPosition(vec3(0, 0, 0));
    cursor.setScale(vec3(GAP_RECT * 2));
    for (int i = -TILE_WORLD_SIZE; i < TILE_WORLD_SIZE; i++) {
        for (int j = -TILE_WORLD_SIZE; j < TILE_WORLD_SIZE; j++) {
            auto &o = tile_.createObject();
            auto t = o.getTransform();
            float y = i + GAP_RECT / 2;
            float z = j + GAP_RECT / 2;
            t.position = vec3(-1, y, z);
            o.setTransform(t);
        }
    }
}
