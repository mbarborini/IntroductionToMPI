#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../Libraries/arrays.h"

void saturated_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
   if (*datatype == MPI_DOUBLE)
   {
      double *in = (double *)invec;
      double *inout = (double *)inoutvec;
      double sum = 0.0;
      for (int i = 0; i < *len; i++)
      {
         sum = in[i] + inout[i];
         inout[i] = (sum > 100) ? 100 : sum;
      }
   }
   else if (*datatype == MPI_INT)
   {
      int *in = (int *)invec;
      int *inout = (int *)inoutvec;
      int sum = 0;
      for (int i = 0; i < *len; i++)
      {
         sum = in[i] + inout[i];
         inout[i] = (sum > 100) ? 100 : sum;
      }
   }
   else if (*datatype == MPI_FLOAT)
   {
      float *in = (float *)invec;
      float *inout = (float *)inoutvec;
      float sum = 0.0;
      for (int i = 0; i < *len; i++)
      {
         sum = in[i] + inout[i];
         inout[i] = (sum > 100) ? 100 : sum;
      }
   }
   else
   {
      fprintf(stderr, "ERROR!!!: Unsupported datatype for saturated_add.\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
   }
}

int main(int argc, char *argv[])
{
   MPI_Init(NULL, NULL);
   int num_of_ranks;
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   int num_elements = 4;
   double *local_values = allocate_1d_double(&num_elements);  

   init_genrand(123);
   for (int i = 0; i < num_elements; i++)
   {
      local_values[i] = (double) genrand_real1() * 10.0;
   }

   MPI_Op MPI_SUM_CAP100;
   MPI_Op_create(*saturated_sum, 1, &MPI_SUM_CAP100);

   double *global_values = allocate_1d_double(&num_elements);  
   MPI_Reduce(local_values, global_values, num_elements, MPI_DOUBLE, MPI_SUM_CAP100, 0, MPI_COMM_WORLD);

   if (mpi_rank == 0)
   {
      printf("--- Custom MPI_Op MPI_SUM_CAP100 Results ---\n");
      for (int i = 0; i < num_elements; i++)
      {
         printf("Element %d (capped at 100) = %f\n", i, global_values[i]);
      }
   }

   MPI_Op_free(&MPI_SUM_CAP100);
   free(local_values);
   free(global_values);

   MPI_Finalize();
   return 0;
}