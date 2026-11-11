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

    int num_rows_columns = 10;
    int total_elements = num_rows_columns * (num_rows_columns + 1) / 2; // 10 elements
    double *recv_buf;
    double **matrix;

    if (mpi_rank == 0)
    {
        matrix = allocate_2d_packed_double(&num_rows_columns, &num_rows_columns);
        int k = 0;
        for (int i = 0; i < num_rows_columns; i++)
        {
            for (int j = 0; j < num_rows_columns; j++)
            {
                matrix[i][j] = (double)k;
                k++;
            }
        }
        print_2d_double(&num_rows_columns, &num_rows_columns, matrix);
    }

    int *blocklenghts = allocate_1d_int(&num_rows_columns);
    int *displacements = allocate_1d_int(&num_rows_columns);
    for (int i = 0; i < num_rows_columns; i++)
    {
        blocklenghts[i] = num_rows_columns - i;        // Shrinking row length (4, 3, 2, 1)
        displacements[i] = (i * num_rows_columns) + i; // Starting offset for each row's diagonal element
    }

    //print_1d_int(&num_rows_columns,blocklenghts);
    //print_1d_int(&num_rows_columns,displacements);

    MPI_Datatype MPI_UPPER_TRIMAT;
    MPI_Type_indexed(num_rows_columns, &blocklenghts[0], &displacements[0], MPI_DOUBLE, &MPI_UPPER_TRIMAT);
    MPI_Type_commit(&MPI_UPPER_TRIMAT);

    if (mpi_rank == 0)
    {
        for (int i = 1; i < num_of_ranks; i++)
        {
            MPI_Send(&matrix[0][0], 1, MPI_UPPER_TRIMAT, i, i, MPI_COMM_WORLD);
        }
    }
    else
    {
        recv_buf = allocate_1d_double(&total_elements);
        MPI_Recv(&recv_buf[0], total_elements, MPI_DOUBLE, 0, mpi_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    
    for (int i = 1; i < num_of_ranks; i++)
    {
        if (mpi_rank == i)
        {
            printf("Vector from rank %d : ", mpi_rank);
            for (int j = 0; j < total_elements; j++)
            {
                printf(" %4.1f ", recv_buf[j]);
            }
        printf("\n");
        }
    }

    MPI_Type_free(&MPI_UPPER_TRIMAT);

    MPI_Finalize();
    return 0;
}