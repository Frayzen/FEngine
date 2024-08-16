#pragma once

#include "simulation/simulation.hh"
class PyscSimulation : public Simulation {
  public:
    void update(double deltaTime) override;
    void init() override;

    PyscSimulation();
    float normalizeScaleFactor = 0.9f;
    float distFactor = 0.001f;

    void updateOverscaleFactor();

    Mesh sun_;
    Mesh earth_;
};
