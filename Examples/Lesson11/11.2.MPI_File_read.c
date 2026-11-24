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

   int n_local_elements = 4;
   int n_global_elements = n_local_elements * num_of_ranks;
   char *filename = "uniform_array.bin";
   double *local_data = allocate_1d_double(&n_local_elements);

   MPI_File fh;
   MPI_File_open( MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

   MPI_Offset file_size = 0;
   int double_size = 0;
   MPI_Type_size(MPI_DOUBLE, &double_size);
   // MPI_File_get_size is an MPI I/O routine that 
   // returns the current size, in bytes, of the file 
   // associated with a given file handle.
   MPI_File_get_size(fh, &file_size);

   MPI_Offset expected_local_bytes = (MPI_Offset)n_global_elements * (MPI_Offset)double_size;
   if (mpi_rank == 0 && file_size < expected_local_bytes)
   {
      printf("Warning: %s has %lld bytes, expected at least %lld bytes\n",
              filename, (long long) file_size, (long long) expected_local_bytes);
   }

   MPI_Offset offset = (MPI_Offset)mpi_rank * (MPI_Offset)n_local_elements * (MPI_Offset)double_size;

   MPI_File_seek(fh, offset, MPI_SEEK_SET);

   MPI_Status status;
   MPI_File_read(fh, local_data, n_local_elements, MPI_DOUBLE, &status);

   int read_count = 0;
   MPI_Get_count(&status, MPI_DOUBLE, &read_count);
   if (read_count != n_local_elements)
   {
      printf("Rank %d: expected %d doubles, read %d\n",
              mpi_rank, n_local_elements, read_count);
   }

   printf("Rank %d read [%.1f", mpi_rank, local_data[0]);
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