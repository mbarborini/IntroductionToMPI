#ifndef ARRAYS_H
#define ARRAYS_H

double* allocate_1d_double(int const elements);
double** allocate_2d_double(int const rows, int const columns);
double* allocate_2d_double_blocked(int const rows, int const columns);
double* free_1d_double(double* const vector);
double** free_2d_double(double** const matrix, int const rows);
double* free_2d_double_blocked(double* const matrix);
void print_1d_double(double const* const vector, int const elements, int const mpi_rank);
void print_2d_double(double** const matrix, int const rows, int const columns, int const mpi_rank);
void print_2d_double_blocked(double const* const matrix, int const rows, int const columns, int const mpi_rank);
void intialize_1d_double(double* const vector, int const elements);
void intialize_2d_double(double** const matrix, int const rows, int const columns);
void intialize_2d_double_blocked(double* const matrix, int const rows, int const columns);
void set_initilize_rand_seed(unsigned int const seed);
double get_double_rand();
void initialize_1d_double_rand(double* const vector, int const elements);
void initialize_2d_double_blocked_rand(double* const matrix, int const rows, int const columns);

#endif
