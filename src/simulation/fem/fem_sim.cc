#include "fem_sim.hh"
#include "constants.hh"
#include "object/object.hh"
#include "object/ray.hh"
#include "simulation/fem/fem_consts.hh"
#include "simulation/fem/fem_gui.hh"
#include "simulation/simulation.hh"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

FemSimulation::FemSimulation()
    : Simulation(true), fem_mesh_(),
      tile_(Mesh::generate2DRect(SQR_SIZE, SQR_SIZE)),
      selector_(Mesh::generateSphere(10, 10)),
      selectIndicator_(Mesh::generate2DRect(GAP_RECT * 5, GAP_RECT * 5)) {
    attachGUI(new FemGUI(*this));
    tile_.getMaterials()[0].setColor(vec3(1.0f, 1.0f, 1.0f), true);
    selector_.getMaterials()[0].setColor(vec3(1.0f, 0.0f, 0.0f), true);
    selectIndicator_.getMaterials()[0].setColor(vec3(0.0f, 0.0f, 1.0f), true);
    bgColor = vec4(0, 0, 0, 0);
    cam.speed = 0.2;
    registerMesh(selector_);
    registerMesh(selectIndicator_);
    registerMesh(fem_mesh_.getMesh());
    registerMesh(tile_);
    cam.position = vec3(-8, 0, 0);
}

void FemSimulation::keyCallback(int key, int action) {
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        toggle2d();
}

void FemSimulation::mouseButtonCallback(int button, int action) {
    static int clicked = 0;
    static vec3 previous = vec3(MAXF);
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
        clicked = 0;
        previous = vec3(MAXF);
    }
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        vec3 cur = selector_.getObjects()[0].getTransform().position;
        if (cur == previous)
            return;
        clicked++;
        if (clicked == 2) {
            fem_mesh_.add_beam(cur, previous);
            clicked = 1;
        }
        previous = cur;
    }
    Object &selected = selectIndicator_.getObjects()[0];
    selected.setPosition(previous);
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
    Object &cursor = selector_.createObject();
    cursor.setPosition(vec3(0, 0, 0));
    cursor.setScale(vec3(GAP_RECT * 2));
    Object &selected = selectIndicator_.createObject();
    selected.setPosition(vec3(MAXF));
    for (int i = -TILE_WORLD_SIZE; i < TILE_WORLD_SIZE; i++) {
        for (int j = -TILE_WORLD_SIZE; j < TILE_WORLD_SIZE; j++) {
            auto &o = tile_.createObject();
            auto t = o.getTransform();
            float y = i + SQR_SIZE / 2 + GAP_RECT / 2;
            float z = j + SQR_SIZE / 2 + GAP_RECT / 2;
            t.position = vec3(0, y, z);
            o.setTransform(t);
        }
    }
    fem_mesh_.reset();
    mouseButtonCallback(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS);
}
