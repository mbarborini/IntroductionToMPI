#include "array.h"

#include <stdlib.h>
#include <stdio.h>

// Allocate empty matrix
double** allocate_2d_double(int const rows, int const columns)
{
    if (rows <= 0 || columns <= 0) {
        return NULL;
    }

    // 1. Allocate memory for the row pointers (an array of int*)
    double** matrix = (double**) malloc(rows * sizeof(double*));

    // 2. Allocate memory for the elements of each row (and initialize to 0)
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*) calloc(columns, sizeof(double));
    }
    return matrix;
}

// Allocate empty matrix (consecutive elements)
double* allocate_2d_double_blocked(int const rows, int const columns)
{
    if (rows <= 0 || columns <= 0) {
        return NULL;
    }

    /* allocate the n*m contiguous items (and initialize to 0) */
    double* matrix = (double*) calloc(rows * columns, sizeof(double));

    return matrix;
}

// Allocate empty vector
double* allocate_1d_double(int const elements)
{
    if (elements <= 0) {
        return NULL;
    }

    // 1. Allocate memory for the row pointers (an array of double*)
    double* vector = (double*) calloc(elements, sizeof(double));

    return vector;
}

double* free_1d_double(double* const vector)
{
    free(vector);
    return NULL;
}

double** free_2d_double(double** const matrix, int const rows)
{
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
	}
	free(matrix);

	return NULL;
}

double* free_2d_double_blocked(double* const matrix)
{
    free(matrix);
	return NULL;
}

// Print matrix
void print_2d_double(double** const mat, int const rows, int const columns, int const mpi_rank)
{
    printf("Matrix from rank %d : ", mpi_rank);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printf(" %4.2f ", mat[i][j]);
        }
    }
    printf("\n");
}

void print_2d_double_blocked(double const* const mat, int const rows, int const columns, int const mpi_rank)
{
    printf("Matrix from rank %d : ", mpi_rank);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printf(" %4.2f ", mat[i*columns + j]);
        }
    }
    printf("\n");
}

// Print vector
void print_1d_double(double const* const vector, int const elements, int const mpi_rank)
{
    printf("Vector from rank %d : ", mpi_rank);
    for (int i = 0; i < elements; i++) {
        printf(" %4.2f ", vector[i]);
    }
    printf("\n");
}

// Initialize vector elements
void intialize_1d_double(double* const vector, int const elements)
{
    for (int i = 0; i < elements; i++) {
        vector[i] = (double) i;
    }
}

// Initialize Matrix elements
void intialize_2d_double(double** const matrix, int const rows, int const columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double) (i * columns + j);
        }
    }
}

void intialize_2d_double_blocked(double* const matrix, int const rows, int const columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i*columns + j] = (double) (i * columns + j);
        }
    }
}

void set_initilize_rand_seed(unsigned int const seed)
{
    srand(seed);
}

double get_double_rand()
{
    return rand() % 100;
}

void initialize_1d_double_rand(double* const vector, int const elements)
{
    for (int i = 0; i < elements; i++) {
        vector[i] = rand() % 100;
    }
}

void initialize_2d_double_blocked_rand(double* const matrix, int const rows, int const columns)
{
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[j+i*rows] = rand() % 100;
        }
    }
}
