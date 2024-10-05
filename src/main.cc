#include "simulation/fem/fem_sim.hh"
int main() {
    Simulation::setup();
    FemSimulation s = FemSimulation();
    s.run();
}
