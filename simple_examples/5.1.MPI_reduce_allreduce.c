#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#include "arrays.h"

int main(int argc, char* argv[])
{
    int num_of_ranks;
    int mpi_rank;

    double* vector = NULL;
    double* vector_sum = NULL;
    int number_of_elements;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    // First rank initializes the variables' value
    if (mpi_rank == 0) {
        number_of_elements = 5;
        vector = allocate_1d_double(number_of_elements);
        intialize_1d_double(vector, number_of_elements);
    }
    // Print the initial vector allocated only by rank 0.
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
        print_1d_double(vector, number_of_elements, mpi_rank);
    }
    sleep(1);

    /* 
       1. Bcasting the number of elements.
       2. the other ranks allocate the vector.
       3. Bcasting the vector.
     */
    MPI_Bcast(&number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (mpi_rank != 0) {
        vector = allocate_1d_double(number_of_elements);
    }
    MPI_Bcast(vector, number_of_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /*
       1. Rank 0 Allocates the vector that contains the sum of all vectors.
       2. Calling MPI reduce with MPI_SUM operation. 
     */
    if (mpi_rank == 0) {
        vector_sum = allocate_1d_double(number_of_elements);
    }
    MPI_Reduce(vector, vector_sum, number_of_elements, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the sum vector allocated only by rank 0.
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
        print_1d_double(vector_sum, number_of_elements, mpi_rank);
    }
    sleep(1);

    /*
       1. All ranks except Rank 0 allocate the sum vector.
       2. Calling MPI Allreduce with MPI_SUM operation. 
     */
    if (mpi_rank != 0) {
        vector_sum = allocate_1d_double(number_of_elements);
    }
    MPI_Allreduce(vector, vector_sum, number_of_elements, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Print all sum vectors after Allreduce
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
    }
    print_1d_double(vector_sum, number_of_elements, mpi_rank);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
