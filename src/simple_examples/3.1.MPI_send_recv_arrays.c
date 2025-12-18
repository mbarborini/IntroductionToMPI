#include <stdio.h>
#include <mpi.h>

#include "array.h"

int main(int argc, char* argv[])
{
    int num_of_ranks;
    int mpi_rank;

    double* vector;
    int number_of_elements;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    /* 
    1. Rank 0 initializes the variables' values
    2. Rank 0 Sends information to all other ranks in a loop
    */ 
    if (mpi_rank == 0) {
        number_of_elements = 5;
        vector = allocate_1d_double(number_of_elements);
        intialize_1d_double(vector, number_of_elements);

        for (int i = 1; i < num_of_ranks; i++) {
            MPI_Send(&number_of_elements, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(vector, number_of_elements, MPI_DOUBLE, i, i+num_of_ranks, MPI_COMM_WORLD);
        }
    }
    
    /* 
    1. The other ranks receive first the information regarding the number of elements
    2. Allocate the vectors needed to receive the vector
    3. The other ranks read receive the vector from rank 0
    */ 
    if (mpi_rank != 0) {
        MPI_Recv(&number_of_elements, 1, MPI_INT, 0, mpi_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        vector = allocate_1d_double(number_of_elements);
        printf("MPI rank %d Received from %d, the number of elements : %d\n",mpi_rank, 0, number_of_elements);
        MPI_Recv(vector, number_of_elements, MPI_DOUBLE, 0, mpi_rank+num_of_ranks, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Print vectors
    print_1d_double(vector, number_of_elements, mpi_rank);

    vector = free_1d_double(vector);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
