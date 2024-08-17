#include "pysc_sim.hh"
#include "object/transform.hh"
#include "simulation/physics/pysc_gui.hh"
#include <glm/fwd.hpp>

// 1 unit = 10 thousand km
#define UNIT_TO_KM 10000.0

#define SUN_RADIUS 695000
#define EARTH_DIST_SUN 150000000
#define EARTH_RADIUS 6350

void PyscSimulation::init() {
    sun_.createObject();
    Object &earth = earth_.createObject();
    auto t = earth.getTransform();
    t.position =
        vec3(-distFactor * EARTH_DIST_SUN / UNIT_TO_KM, 0.0f, 0.0f);
    earth.setTransform(t);
    updateOverscaleFactor();
}

void PyscSimulation::update(double deltaTime) { (void)deltaTime; }

PyscSimulation::PyscSimulation()
    : Simulation(), sun_(Mesh::generateSphere(100, 100)),
      earth_(Mesh::generateSphere(100, 100)) {
    cam.position.x = -distFactor * EARTH_DIST_SUN / UNIT_TO_KM;
    cam.speed = 0.1f;
    sun_.getMaterials()[0].setColor(vec3(1.0f, 0.0f, 0.0f));
    earth_.getMaterials()[0].setColor(vec3(0.0f, 0.0f, 1.0f));
    registerMesh(sun_);
    registerMesh(earth_);
    bgColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    attachGUI(new PyscGUI(*this));
}
#define CONVERGE_VALUE (EARTH_RADIUS / UNIT_TO_KM)
#define INTERPOLATE(a, b, t) ((a) * (1 - t) + (b) * (t))
void PyscSimulation::updateOverscaleFactor() {

    auto sunScale = INTERPOLATE(SUN_RADIUS / UNIT_TO_KM, CONVERGE_VALUE,
                                normalizeScaleFactor);
    sun_.getObjects()[0].setScale(vec3(sunScale));
    auto earthScale = INTERPOLATE(EARTH_RADIUS / UNIT_TO_KM, CONVERGE_VALUE,
                                  normalizeScaleFactor);
    earth_.getObjects()[0].setScale(vec3(earthScale));
}
