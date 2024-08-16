#pragma once

#include "simulation/physics/pysc_sim.hh"
#include "gui/gui.hh"
class PyscGUI : public GUI {
  public:
    PyscGUI(PyscSimulation &sim) : GUI(sim) {}
    void update() override;
};
