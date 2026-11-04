#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "../Libraries/arrays.h"

int main(int argc, char *argv[])
{
   MPI_Init(NULL, NULL);
   int num_of_ranks;
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   double *global_vector;
   double *local_vector;
   int block_size = 2;
   int num_global_elements = num_of_ranks * block_size;

   global_vector = allocate_1d_double(&num_global_elements);
   local_vector = allocate_1d_double(&num_global_elements);

   int k = 0;
   for (int i = 0; i < num_of_ranks; i++)
   {
      for (int j = 0; j < block_size; j++)
      {
         local_vector[k] = (double) i+mpi_rank*10;
         k++;
      }
   }

   MPI_Alltoall(&local_vector[0], block_size, MPI_DOUBLE, &global_vector[0], block_size, MPI_DOUBLE, MPI_COMM_WORLD);


   for (int i = 0; i < num_of_ranks; i++)
   {
      if (mpi_rank == i)
      {
         printf("Rank %d: \n", mpi_rank);
         print_1d_double(&num_global_elements, local_vector);
         print_1d_double(&num_global_elements, global_vector);
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}