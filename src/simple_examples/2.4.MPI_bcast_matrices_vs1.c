#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#include "array.h"

int main(int argc, char* argv[])
{
    int num_of_ranks;
    int mpi_rank;

    double** matrix = NULL;
    int number_of_rows, number_of_columns;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    // First rank initializes the variable's value
    if (mpi_rank == 0) {
        number_of_rows = 2;
        number_of_columns = 2;
        matrix = allocate_2d_double(number_of_rows, number_of_columns);
        intialize_2d_double(matrix, number_of_rows, number_of_columns);
    }

    MPI_Bcast(&number_of_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&number_of_columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (mpi_rank != 0) {
        matrix = allocate_2d_double(number_of_rows, number_of_columns);
    }

    if (mpi_rank == 0) {
         printf("-----------------------------------------------------------\n");
    }
    print_2d_double(matrix, number_of_rows, number_of_columns, mpi_rank);
    sleep(1);

    // Since the memory of the matrix is not consecutive Bcast each row separately
    for (int i = 0; i < number_of_rows; i++) {
        MPI_Bcast(matrix[i], number_of_columns, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
    }
    print_2d_double(matrix, number_of_rows, number_of_columns, mpi_rank);

    matrix = free_2d_double(matrix, number_of_rows);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
