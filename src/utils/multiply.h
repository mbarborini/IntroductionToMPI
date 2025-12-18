#ifndef MULTIPLY_H
#define MULTIPLY_H

#include <stdbool.h>

void multiply_matrices(
    int const m, int const k, int const n,
    double const* const A, double const* const B, double* const C);
bool is_product(
    int const m, int const k, int const n,
    double* A, double* B, double* C,
    double epsilon);

#endif
