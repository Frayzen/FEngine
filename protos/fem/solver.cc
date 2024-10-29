#include <cmath>
#include <iostream>
#include <iomanip> // for std::setprecision

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

