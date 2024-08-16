#pragma once

#include "simulation/particles/ptc_sim.hh"
#include "gui/gui.hh"

class PtcGUI : public GUI {
  public:
    PtcGUI(PtcSimulation &sim) : GUI(sim) {}
    void update() override;
};
