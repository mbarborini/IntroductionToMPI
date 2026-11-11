#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int num_of_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    if (num_of_ranks != 12)
    {
        MPI_Finalize();
        return 0;
    }
    int ranks_per_dims[2] = {3, 4};  // 3x4 grid (12 processes)
    int periods[2] = {0, 0};         // Non-periodic boundaries
    int reorder = 1;
    MPI_Comm MPI_COMM_2D;

    // Create 2D Cartesian Communicator
    MPI_Cart_create(MPI_COMM_WORLD, 2, ranks_per_dims, periods, reorder, &MPI_COMM_2D);

    int rank, coords[2], left, right, up, down;
    MPI_Comm_rank(MPI_COMM_2D, &mpi_rank);
    // Find the coordinates of a give rank
    MPI_Cart_coords(MPI_COMM_2D, mpi_rank, 2, coords);

    // Find neighbors along dimension 1 (columns) 0 (rows)
    MPI_Cart_shift(MPI_COMM_2D, 0, 1, &left, &right);
    MPI_Cart_shift(MPI_COMM_2D, 1, 1, &down, &up);

    for (int i = 0; i < num_of_ranks; i++)
    {
        if (i == mpi_rank)
        {
            printf("Rank %d at (%d, %d) -> Left: %d, Right: %d, Down: %d, Up: %d\n",
                   mpi_rank, coords[0], coords[1], left, right, down, up);
        }
    }

    MPI_Finalize();
    return 0;
}
