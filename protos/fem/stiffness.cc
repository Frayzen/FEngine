#include <iostream>
#include <iomanip>
#include <cmath>
#include "solver.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // For matrix transformations
#include <glm/gtc/type_ptr.hpp>         // For value_ptr

const glm::mat4 local_k_init =
    glm::mat4(1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
              1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

void print_matrix(const glm::mat4 &matrix) {
    std::cout << std::fixed
              << std::setprecision(
                     3); // Set fixed-point notation and 3 decimal places
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(8) << matrix[i][j]
                      << " "; // Set width for alignment
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

float calculate_theta(const glm::vec2 &node1, const glm::vec2 &node2) {
    glm::vec2 diff = node2 - node1;
    return atan2(diff.y, diff.x); // Calculate angle in radians
}

void build_rotation_matrix(float theta, glm::mat4 &rotation_matrix) {
    if (theta == 0.0) {
        rotation_matrix = glm::mat4(1.0f);
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
    glm::mat4 temp_k = local_k_init * rotation_matrix;
    global_k = glm::transpose(rotation_matrix) * temp_k;
    print_matrix(global_k);
}

void calculate_global_stiffness_truss(glm::uvec2 beams[], glm::mat4 beam_ks[],
                                      float K_global[6][6]) {

    for (int i = 0; i < 3; ++i) {
        uint n1 = beams[i][0];
        uint n2 = beams[i][1];
        std::cout << "beam = " << n1 << "-" << n2 << std::endl;

        uint n1_dof_x = n1 * 2;
        uint n1_dof_y = n1 * 2 + 1;
        uint n2_dof_x = n2 * 2;
        uint n2_dof_y = n2 * 2 + 1;
        std::cout << n1_dof_x << " " << n1_dof_y << " " << n2_dof_x << " "
                  << n2_dof_y << std::endl;
        uint dofs[] = {n1_dof_x, n1_dof_y, n2_dof_x, n2_dof_y};
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                K_global[dofs[j]][dofs[k]] += beam_ks[i][j][k];
            }
        }
    }
}

int main() {
#include <iomanip>
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    glm::vec2 node1(0, 0);             // Node 1
    glm::vec2 node2(0.5, sqrt(3) / 2); // Node 2
    glm::vec2 node3(1, 0);             // Node 3

    glm::uvec2 beams[] = {glm::uvec2(0, 1), glm::uvec2(1, 2), glm::uvec2(0, 2)};

    glm::mat4 global_k1, global_k2, global_k3;

    calculate_global_stiffness_beam(node1, node2, global_k1); // Beam 1-2
    calculate_global_stiffness_beam(node2, node3, global_k2); // Beam 2-3
    calculate_global_stiffness_beam(node1, node3, global_k3); // Beam 3-1

    glm::mat4 beam_ks[3] = {global_k1, global_k2, global_k3};
    float K_global[6][6] = {{0.0f}};
    calculate_global_stiffness_truss(beams, beam_ks, K_global);

    std::cout << "K GLOBAL " << std::endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            std::cout << K_global[i][j] << " ";
        }
        std::cout << std::endl;
    }

    float known_forces[6] = {0, -1, 0, -100, -1, -1};

    int count_non_zeros = 0;
    for (int i = 0; i < 6; i++)
        if (known_forces[i] != -1) // zero displacement
            count_non_zeros++;

    float *A = new float[count_non_zeros * count_non_zeros];
    float *B = new float[count_non_zeros];

    // fill A and B accordingly
    int curi = 0;
    for (int i = 0; i < 6; i++) // loop on all rows
    {
        if (known_forces[i] == -1)
            continue;
        // if non zero displacement
        for (int j = 0; j < count_non_zeros; j++)
            A[curi * count_non_zeros + j] =
                K_global[i][6 - count_non_zeros + j];
        B[curi] = known_forces[i];
        curi++;
    }

    std::cout << "A IS" << std::endl;
    for (int i = 0; i < count_non_zeros; ++i) {
        for (int j = 0; j < count_non_zeros; ++j) {
            std::cout << A[i * count_non_zeros + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "B IS" << std::endl;
    for (int i = 0; i < count_non_zeros; ++i)
        std::cout << B[i] << " ";
    std::cout << std::endl;

    gaussianElimination(A, B, count_non_zeros);

    // Output the solution
    std::cout << "Solution x:" << std::endl;
    for (int i = 0; i < count_non_zeros; ++i) {
        std::cout << B[i] << std::endl;
    }

    return 0;
}
