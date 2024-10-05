#pragma once

#include "object/object.hh"
#include "simulation/simulation.hh"
class FemSimulation : public Simulation {
  public:
    FemSimulation();
    void update(double deltaTime);
    void init();

  private:
    Mesh tile_;
    Mesh beam_;
    Mesh selector_;
};
