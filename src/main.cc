#include "simulation/particles/ptc_sim.hh"
int main() {
    Simulation::setup();
    PtcSimulation s = PtcSimulation();
    s.run();
}
