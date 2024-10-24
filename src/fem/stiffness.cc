#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For matrix transformations
#include <glm/gtc/type_ptr.hpp>        // For value_ptr

const float A = 0.1;   // Cross-sectional area
const float L = 1.0;   // Length of the beams
const float E = 200.0; // Young's Modulus

const glm::mat4 local_k = glm::mat4(0.0f);
const glm::mat4 local_k_init = glm::mat4(1.0f, 0.0f, -1.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 0.0f,
                                         -1.0f, 0.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 0.0f);

// Print function for glm::mat4
void print_matrix(const glm::mat4 &matrix) {
    std::cout << std::fixed << std::setprecision(3); // Set fixed-point notation and 3 decimal places
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(8) << matrix[i][j] << " "; // Set width for alignment
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

float calculate_theta(const glm::vec2 &node1, const glm::vec2 &node2) {
    glm::vec2 diff = node2 - node1;
    return atan2(diff.y, diff.x); // Calculate angle in radians
}

glm::mat4 identity_mat() {
    return glm::mat4(1.0f); // Identity matrix using glm
}

void build_rotation_matrix(float theta, glm::mat4 &rotation_matrix) {
    if (theta == 0.0) {
        rotation_matrix = identity_mat();
        return;
    }
    float C = cos(theta);
    float S = sin(theta);

    rotation_matrix[0] = glm::vec4(C * C, C * S, -C * C, -C * S);
    rotation_matrix[1] = glm::vec4(C * S, S * S, -C * S, -S * S);
    rotation_matrix[2] = glm::vec4(-C * C, -C * S, C * C, C * S);
    rotation_matrix[3] = glm::vec4(-C * S, -S * S, C * S, S * S);
}

void calculate_global_stiffness_beam(const glm::vec2 &node1,
                                     const glm::vec2 &node2,
                                     glm::mat4 &global_k) {
    float theta = calculate_theta(node1, node2);
    std::cout << "theta = " << theta << std::endl;

    glm::mat4 rotation_matrix;
    build_rotation_matrix(theta, rotation_matrix);

    // Step 1: Calculate local_k * rotation_matrix
    glm::mat4 temp_k = local_k_init * rotation_matrix;

    // Step 2: Calculate rotation_matrix^T * temp_k
    global_k = glm::transpose(rotation_matrix) * temp_k;

    print_matrix(global_k);
}

int main() {
    // Define the nodes of the truss
    glm::vec2 node1(0, 0);             // Node 1
    glm::vec2 node2(0.5, sqrt(3) / 2); // Node 2
    glm::vec2 node3(1, 0);             // Node 3

    // Calculate global stiffness for each beam
    glm::mat4 global_k1, global_k2, global_k3;

    calculate_global_stiffness_beam(node1, node2, global_k1); // Beam 1-2
    calculate_global_stiffness_beam(node2, node3, global_k2); // Beam 2-3
    calculate_global_stiffness_beam(node1, node3, global_k3); // Beam 3-1

    // Now we need to assemble the global stiffness matrix for the truss
    /* glm::mat4 K_global = glm::mat4(0.0f); // Adjust based on the number of nodes and DOFs */

    /* // Assemble the global stiffness matrix (assuming a specific ordering) */
    /* for (int i = 0; i < 4; ++i) { */
    /*     for (int j = 0; j < 4; ++j) { */
    /*         K_global[i][j] += global_k1[i][j];         // For Beam 1-2 */
    /*         K_global[i + 2][j + 2] += global_k2[i][j]; // For Beam 2-3 */
    /*         K_global[i + 1][j + 1] += global_k3[i][j]; // For Beam 3-1 */
    /*     } */
    /* } */

    return 0;
}

