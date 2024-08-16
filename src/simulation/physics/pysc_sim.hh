#pragma once

#include "simulation/simulation.hh"
class PyscSimulation : public Simulation {
  public:
    void update(double deltaTime) override;
    void init() override;
    PyscSimulation();

  private:
    Mesh sun_;
    Mesh earth_;
};
