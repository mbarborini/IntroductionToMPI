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

   MPI_File fh;
   MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

   int double_size = 0;
   MPI_Type_size(MPI_DOUBLE, &double_size);
   MPI_Offset offset = (MPI_Offset)mpi_rank * (MPI_Offset)n_local_elements * (MPI_Offset)double_size;

   MPI_Status status;
   MPI_File_read_at_all(fh, offset, local_data, n_local_elements, MPI_DOUBLE, &status);

   int read_count = 0;
   MPI_Get_count(&status, MPI_DOUBLE, &read_count);
   if (read_count != n_local_elements)
   {
      printf("Rank %d: read %d of %d doubles\n", mpi_rank, read_count, n_local_elements);
   }

   printf("Rank %d (offset %lld) read [%.1f", mpi_rank, (long long)offset, local_data[0]);
   for (int i = 1; i < read_count; i++)
   {
      printf(", %.1f", local_data[i]);
   }
   printf("]\n");

   MPI_File_close(&fh);
   free(local_data);
   MPI_Finalize();
   return 0;
}