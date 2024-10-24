#pragma once

#include "simulation/fem/fem_sim.hh"
#include "gui/gui.hh"

class FemGUI : public GUI {
  public:
    FemGUI(FemSimulation &sim) : GUI(sim) {}
    void update() override;
};
