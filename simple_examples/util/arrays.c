#include "arrays.h"

#include <stdio.h>
#include <stdlib.h>

// Allocate empty matrix
double** allocate_2d_double(int rows, int columns)
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
double* allocate_2d_double_blocked(int rows, int columns)
{
    if (rows <= 0 || columns <= 0) {
        return NULL;
    }

    /* allocate the n*m contiguous items (and initialize to 0) */
    double* matrix = (double*) calloc(rows * columns, sizeof(double));

    return matrix;
}

// Allocate empty vector
double* allocate_1d_double(int elements)
{
    if (elements <= 0) {
        return NULL;
    }

    // 1. Allocate memory for the row pointers (an array of double*)
    double* vector = (double*) calloc(elements, sizeof(double));
    return vector;
}

// Print matrix
void print_2d_double(double** mat, int rows, int columns, int mpi_rank)
{
    printf("Matrix from rank %d : ", mpi_rank);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printf(" %4.2f ", mat[i][j]);
        }
    }
    printf("\n");
}

void print_2d_double_blocked(double* mat, int rows, int columns, int mpi_rank)
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
void print_1d_double(double* vector, int elements, int mpi_rank)
{
    printf("Vector from rank %d : ", mpi_rank);
    for (int i = 0; i < elements; i++) {
        printf(" %4.2f ", vector[i]);
    }
    printf("\n");
}

// Initialize vector elements
void intialize_1d_double(double* vector, int elements)
{
    for (int i = 0; i < elements; i++) {
        vector[i] = (double) i;
    }
}

// Initialize Matrix elements
void intialize_2d_double(double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double) (i * columns + j);
        }
    }
}

void intialize_2d_double_blocked(double* matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i*columns + j] = (double) (i * columns + j);
        }
    }
}
