#ifndef ARRAYS_H
#define ARRAYS_H

double* allocate_1d_double(int elements);
double** allocate_2d_double(int rows, int columns);
double** allocate_2d_double_blocked(int rows, int columns);
void print_1d_double(double* vector, int* elements, int* mpi_rank);
void print_2d_double(double** matrix, int* rows, int* columns, int* mpi_rank);
void intialize_1d_double(double* vector, int* elements);
void intialize_2d_double(double** matrix, int* rows, int* columns);

#endif
