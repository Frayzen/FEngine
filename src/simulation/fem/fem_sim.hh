#pragma once

#include "object/object.hh"
#include "simulation/fem/fem_2d_mesh.hh"
#include "simulation/simulation.hh"
class FemSimulation : public Simulation {
  public:
    FemSimulation();
    void update(double deltaTime) override;
    void init() override;
    void keyCallback(int key, int action) override;
    void mouseButtonCallback(int button, int action) override;

  private:
    FEM2DMesh fem_mesh_;
    Mesh tile_;
    Mesh selector_;
    Mesh selectIndicator_;
};
