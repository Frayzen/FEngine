#include "fem_computation.hh"
#include "simulation/fem/fem_2d_mesh.hh"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <cmath>

void gaussianElimination(float *A, float *B, int n) { // return value is in B
    // Forward elimination
    for (int i = 0; i < n; ++i) {
        // Partial pivoting
        int maxRow = i;
        for (int j = i + 1; j < n; ++j) {
            if (fabs(A[j * n + i]) > fabs(A[maxRow * n + i])) {
                maxRow = j;
            }
        }

        // Swap the rows in A and corresponding elements in B
        if (maxRow != i) {
            for (int k = 0; k < n; ++k) {
                std::swap(A[i * n + k], A[maxRow * n + k]);
            }
            std::swap(B[i], B[maxRow]);
        }

        // Make the elements below the pivot equal to zero
        for (int j = i + 1; j < n; ++j) {
            float ratio = A[j * n + i] / A[i * n + i];
            for (int k = i; k < n; ++k) {
                A[j * n + k] -= ratio * A[i * n + k];
            }
            B[j] -= ratio * B[i];
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            B[i] -= A[i * n + j] * B[j];
        }
        B[i] /= A[i * n + i];
    }
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

    static const glm::mat4 local_k_init =
        glm::mat4(1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
                  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    float theta = calculate_theta(node1, node2);
    glm::mat4 rotation_matrix;
    build_rotation_matrix(theta, rotation_matrix);
    glm::mat4 temp_k = local_k_init * rotation_matrix;
    global_k = glm::transpose(rotation_matrix) * temp_k;
}

void contribute_global_stifness_truss(uvec2 beam, glm::mat4 beam_ks,
                                      float *K_global, int n) {
    uint n1 = beam.x;
    uint n2 = beam.y;

    uint n1_dof_x = n1 * 2;
    uint n1_dof_y = n1 * 2 + 1;
    uint n2_dof_x = n2 * 2;
    uint n2_dof_y = n2 * 2 + 1;
    uint dofs[] = {n1_dof_x, n1_dof_y, n2_dof_x, n2_dof_y};
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
            K_global[dofs[j] * n + dofs[k]] += beam_ks[j][k];
        }
    }
}

std::vector<vec2> compute_displacement(std::vector<FEMPoint> points,
                                       std::vector<uvec2> elems) {
    int n = points.size() * 2;
    float *global_k = new float[n * n];
    float *known_forces = new float[points.size()];
    std::vector<int> ids_non_zeros;

    mat4 local_k;
    int i = 0;
    for (auto &p : points) {
        switch (p.flags) {
        case NONE:
            break;
        case FIXED:
            known_forces[2 * i + 1] = 0;
            known_forces[2 * i] = 0;
            ids_non_zeros.push_back(2 * i);
            ids_non_zeros.push_back(2 * i + 1);
            break;
        case ROLLING_X:
            known_forces[2 * i + 1] = 0;
            ids_non_zeros.push_back(2 * i);
            break;
        case ROLLING_Y:
            known_forces[2 * i] = 0;
            ids_non_zeros.push_back(2 * i + 1);
            break;
        }
    }

    for (unsigned int i = 0; i < elems.size(); i++) {
        FEMPoint &pt1 = points[elems[i].x];
        FEMPoint &pt2 = points[elems[i].y];
        auto node1 = vec2(pt1.coord.z, pt1.coord.y);
        auto node2 = vec2(pt2.coord.z, pt2.coord.y);
        calculate_global_stiffness_beam(node1, node2, local_k);
        contribute_global_stifness_truss(elems[i], local_k, global_k, n);
    }

    int count_non_zeros = ids_non_zeros.size();
    std::vector<float> A(count_non_zeros * count_non_zeros);
    std::vector<float> B(count_non_zeros);

    for (int i = 0; i < count_non_zeros; i++) // loop on all rows
    {
        B[i] = known_forces[ids_non_zeros[i]];
        for (int j = 0; j < count_non_zeros; j++)
            A[i * count_non_zeros + j] =
                global_k[ids_non_zeros[i] * n + ids_non_zeros[j]];
    }

    gaussianElimination(A.data(), B.data(), count_non_zeros);
    delete[] global_k;
    delete[] known_forces;

    std::vector<vec2> displacements(points.size(), vec2(0));

    for (unsigned int i = 0; i < ids_non_zeros.size(); i++)
    {
      if (i % 2 == 0)
        displacements[ids_non_zeros[i]].x = B[i];
      else
        displacements[ids_non_zeros[i]].y = B[i];
    }

    return displacements;
}
