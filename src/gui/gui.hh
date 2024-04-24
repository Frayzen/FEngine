#pragma once

#include <GLFW/glfw3.h>

class Simulation;

class GUI {
  public:
    GUI(Simulation& sim);
    void update();
    void setup();

  private:
    GLFWwindow *win_;
    Simulation& sim_;
};
