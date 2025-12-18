#include <stdio.h>
#ifdef _MPI
#include <mpi.h>
#endif 

int main(int argc, char* argv[])
{
    int num_of_ranks;
    int mpi_rank;

#ifdef _MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
#else 
    num_of_ranks = 1; 
    mpi_rank = 0;
#endif 

    printf("Hello from process %d of %d\n", mpi_rank, num_of_ranks);

#ifdef _MPI
    MPI_Finalize();
#endif 

    return 0;
}
