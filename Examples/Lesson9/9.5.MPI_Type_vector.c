#include <mpi.h>
#include <stdio.h>
#include "../Libraries/arrays.h"

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int num_of_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    int num_rows = 10;
    int num_columns = 5;
    double *column;
    MPI_Datatype MPI_COLUMN;
    MPI_Type_vector(num_rows, 1, num_columns, MPI_DOUBLE, &MPI_COLUMN);
    MPI_Type_commit(&MPI_COLUMN);

    if (mpi_rank == 0)
    {
        double **matrix = allocate_2d_packed_double(&num_rows, &num_columns);
        int k = 0;
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                matrix[i][j] = (double) k;
                k++;
            }
        }
        print_2d_double(&num_rows, &num_columns, matrix);
        for (int i = 0; i < num_of_ranks; i++)
        {
            MPI_Send(&matrix[0][1], 1, MPI_COLUMN, i, i, MPI_COMM_WORLD);
        }
    }
    else
    {
        column = allocate_1d_double(&num_rows);
        MPI_Recv(&column[0], num_rows, MPI_DOUBLE, 0, mpi_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 1; i < num_of_ranks; i++)
    {
        if (mpi_rank == i)
        {
            printf("Vector from rank %d : ", mpi_rank);
            for (int j = 0; j < num_rows; j++)
            {
                printf(" %4.1f ", column[j]);
            }
            printf("\n");
        }
    }

    MPI_Type_free(&MPI_COLUMN);

    MPI_Finalize();
    return 0;
}