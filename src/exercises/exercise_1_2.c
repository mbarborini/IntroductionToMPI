#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>
#include <time.h>
#include <cblas.h>

#include "array.h"

static unsigned int const seed = 1234;
static int const dimensions[] = {32*1, 32*8, 32*64, 32*512};
static int const n_dimensions = sizeof(dimensions)/sizeof(int);
static double const epsilon = 1e-10;

typedef void (*BLAS_DGEMV)(
    int const m, int const n,
    double const alpha, double const* const A, int const ldA,
    double const* const x, int const incx,
    double const beta, double* const y, int const incy
);

static void populate_random_operants(
    int const m, int const n,
    int const seed,
    double* const A, double* const x, double* const y,
    double* const alpha, double* const beta)
{
    set_initilize_rand_seed(seed);

    initialize_1d_double_rand(x, n);
    initialize_1d_double_rand(y, m);
    initialize_2d_double_blocked_rand(A, m, n);

    *alpha = get_double_rand();
    *beta = get_double_rand();
}

static void initialize_problem_operants(
    int const m, int const n,
    double** const A,
    double** const x, double** const y)
{
    *A = allocate_2d_double_blocked(m, n);
    *x = allocate_1d_double(n);
    *y = allocate_1d_double(m);
}

static void destroy_problem_operants(double** const A, double** const x, double** const y)
{
    *A = free_2d_double_blocked(*A);
    *x = free_1d_double(*x);
    *y = free_1d_double(*y);
}

static bool test_DGEMV(int const m, int const n, BLAS_DGEMV dgemv, double const epsilon, unsigned int const seed, double* const duration)
{
    double* A = NULL;
    double* x = NULL;
    double* y = NULL;
    double alpha = 0.0;
    double beta = 0.0;

    initialize_problem_operants(m, n, &A, &x, &y);
    populate_random_operants(m, n, seed, A, x, y, &alpha, &beta);

    double* y_test = allocate_1d_double(m);
    for (int i = 0; i < m; i++) {
        y_test[i] = y[i];
    }

    CBLAS_LAYOUT const layout = CblasColMajor;
    CBLAS_TRANSPOSE const transA = CblasNoTrans;
    int const ldA = m;
    int const incx = 1;
    int const incy = 1;

    cblas_dgemv(layout, transA, 
        m, n,
        alpha, A, ldA,
        x, incx,
        beta, y_test, incy);

    // Time the execution of dgemv
    clock_t const start = clock();
    dgemv(
        m, n,
        alpha, A, ldA,
        x, incx,
        beta, y, incy);
    clock_t const end = clock();

    double err = 0.0;
    for (int i = 0; i < m; i++) {
        double const diff = y[i] - y_test[i];
        err += diff*diff;
    }
    err = sqrt(err);
    
    bool result_is_correct = err < epsilon;

    y_test = free_1d_double(y_test);
    destroy_problem_operants(&A, &x, &y);

    *duration = ((double) (end - start)) / CLOCKS_PER_SEC;

    return result_is_correct;
}

// In the implementation of functions "DGEMV" and "rowwise_DGEMV", replace the
//  call to the BLAS function with your own implementation.
void DGEMV(
    int const m, int const n,
    double const alpha, double const* const A, int const ldA,
    double const* const x, int const incx,
    double const beta, double* const y, int const incy)
{
    CBLAS_LAYOUT const layout = CblasColMajor;
    CBLAS_TRANSPOSE const transA = CblasNoTrans;

    cblas_dgemv(layout, transA, 
        m, n,
        alpha, A, ldA,
        x, incx,
        beta, y, incy);
}

void rowwise_DGEMV(
    int const m, int const n,
    double const alpha, double const* const A, int const ldA,
    double const* const x, int const incx,
    double const beta, double* const y, int const incy)
{
    CBLAS_LAYOUT const layout = CblasColMajor;
    CBLAS_TRANSPOSE const transA = CblasNoTrans;

    cblas_dgemv(layout, transA, 
        m, n,
        alpha, A, ldA,
        x, incx,
        beta, y, incy);
}


static bool generate_operand_dimensions(int* const m, int* const n)
{
    int const max_dim = n_dimensions;
    static int m_dim = 0;
    static int n_dim = 0;

    if (n_dim >= max_dim) {
        return false;
    }

    *m = dimensions[m_dim];
    *n = dimensions[n_dim];

    m_dim++;
    if (m_dim >= max_dim) {
        m_dim = 0;
        n_dim++;
    }

    return true;
}

int main(int argc, char* argv[])
{
    bool all_test_pass = true;

    int n = 0;
    int m = 0;

    while (generate_operand_dimensions(&m, &n)) {
        double columnwise_duration = 0.0;
        bool const test_DGEMV_pass = test_DGEMV(m, n, DGEMV, epsilon, seed, &columnwise_duration);
        if (!test_DGEMV_pass) {
            fprintf(stderr, "DGENV failed for: m=%d, n=%d\n", m, n);
            all_test_pass = false;
        }
        double rowwise_duration = 0.0;
        bool const test_rowwise_DGEMV_pass = test_DGEMV(m, n, rowwise_DGEMV, epsilon, seed, &rowwise_duration);
        if (!test_rowwise_DGEMV_pass) {
            fprintf(stderr, "rowwise_DGEMV failed for: m=%d, n=%d\n", m, n);
            all_test_pass = false;
        }
        printf("Duration of case m=%d, n=%d: columnwise=%lf, rowwise=%lf\n", m, n, rowwise_duration, columnwise_duration);
    }

    if (!all_test_pass) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
