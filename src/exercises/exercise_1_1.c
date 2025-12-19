#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>
#include <cblas.h>

#include "array.h"

static unsigned int const seed = 1234;
static int const dimensions[] = {128*1, 128*2, 128*4, 128*8};
static int const n_dimensions = sizeof(dimensions)/sizeof(int);
static double const epsilon = 1e-10;

typedef void (*BLAS_DAXPY)(
    int const n, double const alpha, double* const x, int const incx, double* const y, int const incy
);

typedef double (*BLAS_DDOT)(
    int const n, double* const x, int const incx, double* const y, int const incy
);

static void populate_random_DDOT_operants(
    int const n,
    int const seed,
    double* const x, double* const y)
{
    set_initilize_rand_seed(seed);

    initialize_1d_double_rand(x, n);
    initialize_1d_double_rand(y, n);
}

static void populate_random_DAXPY_operants(
    int const n,
    int const seed,
    double* const alpha,
    double* const x, double* const y)
{
    set_initilize_rand_seed(seed);

    *alpha = get_double_rand();
    initialize_1d_double_rand(x, n);
    initialize_1d_double_rand(y, n);
}

static void initialize_problem_operants(
    int const n,
    double** const x, double** const y)
{
    *x = allocate_1d_double(n);
    *y = allocate_1d_double(n);
}

static void destroy_problem_operants(double** const x, double** const y)
{
    *x = free_1d_double(*x);
    *y = free_1d_double(*y);
}

static bool test_DAXPY(int const n, BLAS_DAXPY axpy, double const epsilon, unsigned int const seed)
{
    double* x = NULL;
    double* y = NULL;
    double alpha = 0;
    initialize_problem_operants(n, &x, &y);
    populate_random_DAXPY_operants(n, seed, &alpha, x, y);

    double* y_test = allocate_1d_double(n);
    for (int i = 0; i < n; i++) {
        y_test[i] = y[i];
    }

    axpy(n, alpha, x, 1, y, 1);
    cblas_daxpy(n, alpha, x, 1, y_test, 1);
    double err = 0.0;
    for (int i = 0; i < n; i++) {
        double const diff = y[i] - y_test[i];
        err += diff*diff;
    }
    err = sqrt(err);
    
    bool result_is_correct = err < epsilon;

    y_test = free_1d_double(y_test);
    destroy_problem_operants(&x, &y);

    return result_is_correct;
}

static bool test_DDOT(int const n, BLAS_DDOT dot, double const epsilon, unsigned int const seed)
{
    double* x = NULL;
    double* y = NULL;
    initialize_problem_operants(n, &x, &y);
    populate_random_DDOT_operants(n, seed, x, y);

    double const d = dot(n, x, 1, y, 1);
    double const d_test = cblas_ddot(n, x, 1, y, 1);
    
    double const diff = d - d_test;
    double const err = sqrt(diff*diff);
    
    bool result_is_correct = err < epsilon;

    destroy_problem_operants(&x, &y);

    return result_is_correct;
}

// In the implementation of functions "DAXPY" and "DDOT" replace the call to
// the corresponding BLAS function with your own implementation.
void DAXPY(int const n, double const alpha, double* const x, int const incx, double* const y, int const incy)
{
    cblas_daxpy(n, alpha, x, incx, y, incy);
}

double DDOT(int const n, double* const x, int const incx, double* const y, int const incy)
{
    return cblas_ddot(n, x, incx, y, incy);
}

static bool generate_operand_dimension(int* const n)
{
    int const max_dim = n_dimensions;
    static int dim = 0;

    if (dim >= max_dim) {
        return false;
    }

    *n = dimensions[dim];

    dim++;

    return true;
}

int main(int argc, char* argv[])
{
    bool all_tests_pass = true;

    int n = 0;

    while (generate_operand_dimension(&n)) {
        bool const test_DAXPY_pass = test_DAXPY(n, DAXPY, epsilon, seed);
        if (!test_DAXPY_pass) {
            printf("DAXPY failed for: n=%d\n", n);
            all_tests_pass = false;
        }
        bool const test_DDOT_pass = test_DDOT(n, DDOT, epsilon, seed);
        if (!test_DDOT_pass) {
            printf("DDOT failed for: n=%d\n", n);
            all_tests_pass = false;
        }
    }

    if (!all_tests_pass) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
