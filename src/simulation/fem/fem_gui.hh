#pragma once

#include "gui/gui.hh"

class FemGUI : public GUI {
  public:
    FemGUI(Simulation &sim) : GUI(sim) {}
    void update() override;
    ~FemGUI() override {};
};
