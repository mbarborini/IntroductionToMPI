#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int num_of_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    long int extent = 0;       
    long int lower_bound = 0;  
    int size = 0;              
    
    MPI_Type_get_extent(MPI_DOUBLE, &lower_bound, &extent);
    MPI_Type_size(MPI_DOUBLE, &size);

    printf("For MPI_DOUBLE we have\n");
    printf("Lower bound : %ld\n", lower_bound);
    printf("Extent      : %ld\n", extent);
    printf("Size        : %d\n", size);

    MPI_Finalize();
    return 0;
}