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

   int num_global_elements = 80;

   int num_local_elements_per_rank = (int)num_global_elements / num_of_ranks;
   int num_residual_elements = (int)num_global_elements % num_of_ranks;

   if (num_residual_elements == 0)
   {
      local_vector = allocate_1d_double(&num_local_elements_per_rank);
      init_random_vector(mpi_rank, &num_local_elements_per_rank, local_vector);
      if (mpi_rank == 0)
      {
         global_vector = allocate_1d_double(&num_global_elements);
      }
      MPI_Gather(&local_vector[0], num_local_elements_per_rank, MPI_DOUBLE,
                  &global_vector[0], num_local_elements_per_rank, MPI_DOUBLE, 0,
                  MPI_COMM_WORLD);
   }
   else
   {
      if (mpi_rank == 0)
      {
         printf("The number of rows is not divisible by the number of ranks.\n");
         printf("The program will be terminated.\n");
      }
      MPI_Finalize();
      return 1;
   }


   if (mpi_rank == 0)
   {
      printf("Partial vector from rank: %4i \n", mpi_rank);
      print_1d_double(&num_local_elements_per_rank, local_vector);
   }
   for (int i = 1; i < num_of_ranks; i++)
   {
      if (mpi_rank == i)
      {
         MPI_Send(&local_vector[0], num_local_elements_per_rank, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
      }
      else if (mpi_rank == 0)
      {
         MPI_Recv(&local_vector[0], num_local_elements_per_rank, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("Partial vector from rank: %4i \n", i);
         print_1d_double(&num_local_elements_per_rank, local_vector);
      }
   }
   if (mpi_rank == 0)
   {
      printf("\n");
      printf("Global vector:\n");
      print_1d_double(&num_global_elements, global_vector);
   }

   MPI_Finalize();
   return 0;
}
