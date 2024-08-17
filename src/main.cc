#include "simulation/physics/pysc_sim.hh"
int main() {
    Simulation::setup();
    PyscSimulation s = PyscSimulation();
    s.run();
}
