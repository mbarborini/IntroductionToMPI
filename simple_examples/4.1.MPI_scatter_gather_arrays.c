#include <stdio.h>
#include <mpi.h>

#include "arrays.h"

int main(int argc, char* argv[])
{
    int num_of_ranks;
    int mpi_rank;

    double* vector = NULL;
    double* partial_vector = NULL;
    int number_of_elements;
    int number_of_local_elements;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    // First rank initializes variables
    if (mpi_rank == 0) {
        number_of_local_elements = 2;
        number_of_elements = number_of_local_elements * num_of_ranks;
        vector = allocate_1d_double(number_of_elements);
        intialize_1d_double(vector, &number_of_elements);
        print_1d_double(vector, &number_of_elements, &mpi_rank);
    }
    
    // Number of elements in rank 0 becasted to all world
    MPI_Bcast(&number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&number_of_local_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Every rank allocates the receive vector of the partial elements
    partial_vector = allocate_1d_double(number_of_local_elements);
    // Rank 0 distributes original vector in chunks
    MPI_Scatter(&vector[0], number_of_local_elements, MPI_DOUBLE, &partial_vector[0], number_of_local_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print of the partial vectors
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
    }
    print_1d_double(partial_vector, &number_of_local_elements, &mpi_rank);
    sleep(1);

    // Change values of the partial vectors and print again
    for (int i = 0; i < number_of_local_elements; i++) {
        partial_vector[i] = mpi_rank;
    }
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
    }
    print_1d_double(partial_vector, &number_of_local_elements, &mpi_rank);
    sleep(1);

    // Reassemble the information of the partial vectors to rank 0
    MPI_Gather(&partial_vector[0], number_of_local_elements, MPI_DOUBLE, &vector[0], number_of_local_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // All ranks allocate full vector
    if (vector == NULL) {
        vector = allocate_1d_double(number_of_elements);
    }   
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
    }
    print_1d_double(vector, &number_of_elements, &mpi_rank);
    sleep(1);

    // Gather all information in partial vector to complete vector for all ranks
    MPI_Allgather(&partial_vector[0], number_of_local_elements, MPI_DOUBLE, &vector[0], number_of_local_elements, MPI_DOUBLE, MPI_COMM_WORLD);
    if (mpi_rank == 0) {
        printf("-----------------------------------------------------------\n");
    }
    print_1d_double(vector, &number_of_elements, &mpi_rank);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
