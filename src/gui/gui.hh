#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Simulation;

class GUI {
  public:
    GUI(Simulation &sim);
    virtual void update() = 0;
    void setup();
    virtual ~GUI() = default;

  protected:
    GLFWwindow *win_;
    Simulation &sim_;
};
