#include "multiply.h"

#include <stdbool.h>
#include <math.h>

#include "array.h"

void multiply_matrices(
    int const m, int const k, int const n,
    double const* const A, double const* const B, double* const C)
{
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            C[i+j*m] = 0.0;
        }
    }

    for (int j = 0; j < n; j++) {
        for (int l = 0; l < k; l++) {
            for (int i = 0; i < m; i++) {
                C[i+j*m] += A[i+l*m]*B[l+j*k];
            }
        }
    }
}

static double fnorm(int const m, int const n, double* A, double* B)
{
    double sum = 0.0;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            double const diff = A[i+j*m] - B[i+j*m];
            sum += diff*diff;
        }
    }

    return sqrt(sum);
}

bool is_product(
    int const m, int const k, int const n,
    double* A, double* B, double* C,
    double epsilon)
{
    double* Cp = allocate_2d_double_blocked(m, n);
    multiply_matrices(m, k, n, A, B, Cp);

    return fnorm(m, n, C, Cp) < epsilon;
}
