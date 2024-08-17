#pragma once

#include "mesh/mesh.hh"
#include "gui/gui.hh"
#include "object/object.hh"
#include "shader/render.hh"
#include "object/camera.hh"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

using namespace glm;
class Simulation {
  public:
    Camera cam = Camera();

    bool isRunning = true;

    virtual void update(double deltaTime) = 0;
    virtual void init() = 0;

    // Used to reinitialize the simulation
    void restartSimulation();
    void registerMesh(Mesh &m);
    void run();

    static void setup();

    std::vector<std::reference_wrapper<Object>> getObjects();

  protected:
    Simulation();
    ~Simulation();
    void attachGUI(GUI *gui);
    vec4 bgColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);

  private:
    void setupBuffers();
    void setupBuffer(Mesh &m);
    void updateBuffers();
    void udpateBuffer(Mesh &m);
    void updateBboxes();
    void updateBbox(Mesh &m);
    void compute(Mesh &m);

    double lastTime_;

    std::vector<std::reference_wrapper<Mesh>> meshes_ =
        std::vector<std::reference_wrapper<Mesh>>();

    GUI *gui_ = nullptr;
    Render renderer_;
    GLFWwindow *win_;
};
