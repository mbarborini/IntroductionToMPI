#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Libraries/arrays.h"

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int num_of_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    int num_elements = 10;
    int *local_vector = allocate_1d_int(&num_elements);
    if (mpi_rank == 0)
    {
        for (int i = 0; i < num_elements; i++)
        {
            local_vector[i] = i;
        }
    }

    MPI_Bcast(&local_vector[0], num_elements, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < num_of_ranks; i++)
    {
        if (mpi_rank == i)
        {
            printf("Vector from rank %d : ", mpi_rank);
            for (int j = 0; j < num_elements; j++)
            {
                printf(" %d ", local_vector[j]);
            }
            printf("\n");
        }
    }

    MPI_Datatype MPI_INT100;
    MPI_Type_contiguous(num_elements, MPI_INT, &MPI_INT100);
    MPI_Type_commit(&MPI_INT100);

    if (mpi_rank != 0)
    {
        for (int i = 0; i < num_elements; i++)
        {
            local_vector[i] = 0;
        }
    }

    MPI_Bcast(&local_vector[0], 1, MPI_INT100, 0, MPI_COMM_WORLD);

    MPI_Type_free(&MPI_INT100);

    for (int i = 0; i < num_of_ranks; i++)
    {
        if (mpi_rank == i)
        {
            printf("Vector from rank %d : ", mpi_rank);
            for (int j = 0; j < num_elements; j++)
            {
                printf(" %d ", local_vector[j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}