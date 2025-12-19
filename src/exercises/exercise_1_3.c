#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <math.h>
#include <time.h>
#include <cblas.h>

#include "array.h"

static unsigned int const seed = 1234;
static int const dimensions[] = {32*1, 32*4, 32*16, 32*32};
static int const n_dimensions = sizeof(dimensions)/sizeof(int);
static double const epsilon = 1e-10;

typedef void (*BLAS_DGEMM)(
    int const m, int const n, int const k,
    double const alpha, double const* const A,
    int const ldA, double const* const B, int const ldB,
    double const beta, double* const C, int const ldC
);

static void populate_random_operants(
    int const m, int const n, int const k,
    int const seed,
    double* const A, double* const B, double* const C,
    double* alpha, double* beta)
{
    set_initilize_rand_seed(seed);

    initialize_2d_double_blocked_rand(A, m, k);
    initialize_2d_double_blocked_rand(B, k, n);
    initialize_2d_double_blocked_rand(C, m, n);

    *alpha = get_double_rand();
    *beta = get_double_rand();
}

static void initialize_problem_operants(
    int const m, int const n, int const k,
    double** const A, double** const B, double** const C)
{
    *A = allocate_2d_double_blocked(m, k);
    *B = allocate_2d_double_blocked(k, n);
    *C = allocate_2d_double_blocked(m, n);
}

static void destroy_problem_operants(double** const A, double** const B, double** const C)
{
    *A = free_2d_double_blocked(*A);
    *B = free_2d_double_blocked(*B);
    *C = free_2d_double_blocked(*C);
}

static bool test_DGEMM(int const m, int const n, int const k,
    BLAS_DGEMM dgemm,
    double const epsilon, unsigned int const seed, double* const duration)
{
    double* A = NULL;
    double* B = NULL;
    double* C = NULL;
    double alpha = 0.0;
    double beta = 0.0;

    initialize_problem_operants(m, n, k, &A, &B, &C);
    populate_random_operants(m, n, k, seed, A, B, C, &alpha, &beta);


    double* C_test = allocate_2d_double_blocked(m,n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C_test[j+i*m] = C[j+i*m];
        }
    }

    CBLAS_LAYOUT const layout = CblasColMajor;
    CBLAS_TRANSPOSE const transA = CblasNoTrans;
    CBLAS_TRANSPOSE const transB = CblasNoTrans;
    int const ldA = m;
    int const ldB = k;
    int const ldC = m;

    cblas_dgemm(layout, transA, transB,
        m, n, k,
        alpha, A, ldA,
        B, ldB,
        beta, C, ldC);

    // Time the execution of dgemm
    clock_t const start = clock();
    dgemm(
        m, n, k,
        alpha, A, ldA,
        B, ldB,
        beta, C_test, ldC);
    clock_t const end = clock();

    double err = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double const diff = C[j+i*m] - C_test[j+i*m];
            err += diff*diff;
        }
    }
    err = sqrt(err);

    bool result_is_correct = err < epsilon;

    C_test = free_2d_double_blocked(C_test);
    destroy_problem_operants(&A, &B, &C);

    *duration = ((double) (end - start)) / CLOCKS_PER_SEC;

    return result_is_correct;
}

// In the implementation of functions "DGEMM" and "rowwise_DGEMM", replace the
//  call to the BLAS function with your own implementation.
void DGEMM(
    int const m, int const n, int const k,
    double const alpha, double const* const A,
    int const ldA, double const* const B, int const ldB,
    double const beta, double* const C, int const ldC)
{
    CBLAS_LAYOUT const layout = CblasColMajor;
    CBLAS_TRANSPOSE const transA = CblasNoTrans;
    CBLAS_TRANSPOSE const transB = CblasNoTrans;

    cblas_dgemm(layout, transA, transB,
        m, n, k,
        alpha, A, ldA,
        B, ldB,
        beta, C, ldC);
}

void rowwise_DGEMM(
    int const m, int const n, int const k,
    double const alpha, double const* const A,
    int const ldA, double const* const B, int const ldB,
    double const beta, double* const C, int const ldC)
{
    CBLAS_LAYOUT const layout = CblasColMajor;
    CBLAS_TRANSPOSE const transA = CblasNoTrans;
    CBLAS_TRANSPOSE const transB = CblasNoTrans;

    cblas_dgemm(layout, transA, transB,
        m, n, k,
        alpha, A, ldA,
        B, ldB,
        beta, C, ldC);
}


static bool generate_operand_dimensions(int* const m, int* const n, int* const k)
{
    int const max_dim = n_dimensions;
    static int m_dim = 0;
    static int k_dim = 0;
    static int n_dim = 0;

    if (n_dim >= max_dim) {
        return false;
    }

    *m = dimensions[m_dim];
    *k = dimensions[k_dim];
    *n = dimensions[n_dim];

    m_dim++;
    if (m_dim >= max_dim) {
        m_dim = 0;
        k_dim++;
    }
    if (k_dim >= max_dim) {
        k_dim = 0;
        n_dim++;
    }

    return true;
}

int main(int argc, char* argv[])
{
    bool all_test_pass = true;

    int n = 0;
    int m = 0;
    int k = 0;

    while (generate_operand_dimensions(&m, &n, &k)) {
        double columnwise_duration = 0.0;
        bool const test_DGEMM_pass = test_DGEMM(m, n, k, DGEMM, epsilon, seed, &columnwise_duration);
        if (!test_DGEMM_pass) {
            fprintf(stderr, "DGEMM failed for: m=%d, n=%d, k=%d\n", m, n, k);
            all_test_pass = false;
        }
        double rowwise_duration = 0.0;
        bool const test_rowwise_DGEMM_pass = test_DGEMM(m, n, k, rowwise_DGEMM, epsilon, seed, &rowwise_duration);
        if (!test_rowwise_DGEMM_pass) {
            fprintf(stderr, "rowwise_DGEMM failed for: m=%d, n=%d, k=%d\n", m, n, k);
            all_test_pass = false;
        }
        printf("Duration of case m=%d, n=%d, k=%d: columnwise=%lf, rowwise=%lf\n", m, n, k, rowwise_duration, columnwise_duration);
    }

    if (!all_test_pass) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
