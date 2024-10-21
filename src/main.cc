#include "simulation/fem/fem_sim.hh"
#include <iostream>
#include <string>
#include <gtest/gtest.h>

// Function to run the main simulation
void runSimulation() {
    Simulation::setup();
    FemSimulation s = FemSimulation();
    s.run();
}

// Main function
int main(int argc, char** argv) {
    // Check for a command-line argument to run tests
    if (argc > 1 && std::string(argv[1]) == "--test") {
        // Initialize Google Test
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();  // Run all tests
    }

    // If no test argument is provided, run the main simulation
    runSimulation();

    return 0;
}

