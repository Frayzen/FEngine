#include "pysc_sim.hh"
#include "object/transform.hh"
#include "simulation/physics/pysc_gui.hh"
#include <glm/fwd.hpp>

// 1 unit = 10 thousand km
#define UNIT_TO_KM 10000.0
#define OVERSCALE_FACTOR 100.0f


#define SUN_RADIUS 695000
#define EARTH_DIST_SUN 150000000
#define EARTH_RADIUS 6350

void PyscSimulation::init() {
    Object &sun = sun_.createObject();
    auto t = sun.getTransform();
    t.scale = OVERSCALE_FACTOR * vec3(SUN_RADIUS / UNIT_TO_KM);
    sun.setTransform(t);

    Object &earth = earth_.createObject();
    t = earth.getTransform();
    t.position = vec3(-EARTH_DIST_SUN / UNIT_TO_KM, 0.0f, 0.0f);
    t.scale = OVERSCALE_FACTOR * vec3(EARTH_RADIUS / UNIT_TO_KM);
    earth.setTransform(t);
}

void PyscSimulation::update(double deltaTime) { (void)deltaTime; }

PyscSimulation::PyscSimulation()
    : Simulation(), sun_(Mesh::generateSphere(100, 100)), earth_(Mesh::generateSphere(100, 100)) {
    cam.position.x = -(EARTH_DIST_SUN + 800 * EARTH_RADIUS) / UNIT_TO_KM;
    sun_.getMaterials()[0].setColor(vec3(1.0f, 0.0f, 0.0f));
    earth_.getMaterials()[0].setColor(vec3(0.0f, 0.0f, 1.0f));
    registerMesh(sun_);
    registerMesh(earth_);
    bgColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    attachGUI(new PyscGUI(*this));
}
