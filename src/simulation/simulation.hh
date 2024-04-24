#pragma once

#include "mesh/mesh.hh"
#include "gui/gui.hh"
#include "shader/compute.hh"
#include "shader/render.hh"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using namespace glm;
class Simulation {
  public:
    Simulation();
    vec2 screenSize = vec2(2000, 1000);
    vec3 bounds = vec3(10.0f, 10.0f, 0.0f);
    vec2 size = uvec2(10, 11);
    vec2 offset = vec2(1.0f, 1.0f);
    float mass = 1.0f;
    float pressureMultiplier = 100.0f;
    float radius = 1.5f;
    Camera cam;
    bool isRunning = true;
    void restartSimulation();
    void mainLoop();

  private:
    void createObjects();
    void updateBuffers();
    void updateBbox();
    void compute();
    double last_;
    GUI gui_;
    Mesh particleMesh_;
    Mesh boundingMesh_;
    Compute densityCmpt_;
    Compute velocityCpt_;
    Render renderer_;
    GLFWwindow* win_;
};
