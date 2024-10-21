#include <gtest/gtest.h>
#include "fem_2d_mesh.hh"

// Fixture for FEM2DMesh class
class FEM2DMeshTest : public ::testing::Test {
protected:
    FEM2DMesh mesh;

    void SetUp() override {
        // You can initialize common setup for all test cases here if needed
    }

    void TearDown() override {
        // Cleanup if necessary after each test
    }
};

TEST(FEM2DMeshTest, AddBeam) {
    std::cout << "Starting AddBeam test..." << std::endl;

    FEM2DMesh mesh; // Make sure this is properly initialized
    glm::vec3 start(0.0f, 0.0f, 0.0f);
    glm::vec3 end(1.0f, 1.0f, 1.0f);
    
    mesh.add_beam(start, end);

    std::cout << "Finished AddBeam test." << std::endl;
}

