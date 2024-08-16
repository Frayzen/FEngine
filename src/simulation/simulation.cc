#include "simulation.hh"
#include "gui/imgui.h"
#include "gui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cstring>

#define OBJNB(Mesh) (Mesh.getObjects().size())

void Simulation::restartSimulation() {
    isRunning = false;
    for (auto &m : meshes_)
        m.get().clearObjects();
    init();
}

Simulation::Simulation()
    : renderer_(
          Render("assets/shaders/default.vert", "assets/shaders/default.frag")),
      win_(glfwGetCurrentContext()) {
    assert(glfwGetCurrentContext() != nullptr);
    glfwSetTime(0);
    lastTime_ = glfwGetTime();
}

void Simulation::registerMesh(Mesh &m) { meshes_.push_back(m); }

// Main loop
void Simulation::run() {
    init();
    while (!glfwWindowShouldClose(win_)) {

        // BACKGROUND
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TIME UPDATE
        double curTime = glfwGetTime();
        double deltaTime = curTime - lastTime_;
        lastTime_ = curTime;

        // EVENTS
        glfwPollEvents();

        cam.inputs();
        update(deltaTime);

        // RENDER
        for (unsigned long i = 0; i < meshes_.size(); i++)
            meshes_[i].get().render(renderer_, cam);

        if (gui_ != nullptr) {

            gui_->update();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(win_);
    }
}

void Simulation::attachGUI(GUI *gui) {
    gui_ = gui;
    if (gui_ == nullptr)
        return;
    gui->setup();
}
