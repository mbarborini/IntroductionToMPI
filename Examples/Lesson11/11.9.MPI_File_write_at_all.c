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

   int n_local_elements = mpi_rank + 2;
   char *filename = "collective_at.bin";

   double *local_data = allocate_1d_double(&n_local_elements);
   for (int i = 0; i < n_local_elements; i++)
   {
      local_data[i] = (double)mpi_rank;
   }

   MPI_File fh;
   MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

   int double_size = 0;
   MPI_Type_size(MPI_DOUBLE, &double_size);
   MPI_Offset offset = (MPI_Offset)mpi_rank * (MPI_Offset)n_local_elements * (MPI_Offset)double_size;

   MPI_Status status;
   MPI_File_write_at_all(fh, offset, local_data, n_local_elements, MPI_DOUBLE, &status);

   int write_count = 0;
   MPI_Get_count(&status, MPI_DOUBLE, &write_count);
   if (write_count != n_local_elements)
   {
      printf("Rank %d: wrote %d of %d doubles\n", mpi_rank, write_count, n_local_elements);
   }

   MPI_File_close(&fh);
   free(local_data);
   MPI_Finalize();
   return 0;
}