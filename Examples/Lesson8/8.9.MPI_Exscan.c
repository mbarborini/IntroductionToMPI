#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
   MPI_Init(NULL, NULL);
   int num_of_ranks;
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   int local_val = mpi_rank;
   int scan_result = 0;

   MPI_Exscan(&local_val, &scan_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

   printf("Process %d of %d: Local Value = %d, Scan Prefix Sum Result = %d\n",
          mpi_rank, num_of_ranks, local_val, scan_result);

   MPI_Finalize();
   return 0;
}