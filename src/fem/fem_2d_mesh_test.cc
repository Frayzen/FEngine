#include <gtest/gtest.h>
#include "../simulation/fem/fem_2d_mesh.hh"

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

// Test case for adding a beam
TEST_F(FEM2DMeshTest, AddBeam) {
    vec3 v1 = vec3(0.0f, 0.0f, 0.0f);
    vec3 v2 = vec3(1.0f, 0.0f, 0.0f);

    // Add a beam between two points
    mesh.add_beam(v1, v2);

    // Assert that the beam was added (this might depend on how your class stores the beam)
    // For example, check the size of `elems1d_` if you store the beams there
}

// Test case for updating position
TEST_F(FEM2DMeshTest, UpdatePosition) {
    vec3 v1 = vec3(0.0f, 0.0f, 0.0f);
    vec3 v2 = vec3(1.0f, 0.0f, 0.0f);
    mesh.add_beam(v1, v2);

    int nodeId = 0;  // Assume we want to update node 0
    mesh.updatePos(nodeId);

    // Add an assertion to verify that the position was updated correctly
    // For example, check the internal state of the node position after calling updatePos
    // ASSERT_EQ(...);
}

// Add more tests for global stiffness matrix construction, etc.
