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
   for (int i = 0; i < n_local_elements; i++)
   {
      local_data[i] = (double)mpi_rank;
   }

   MPI_File fh;
   MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

   // MPI_Type_size gives the size in bytes
   int double_size = 0;
   MPI_Type_size(MPI_DOUBLE, &double_size);
   // MPI_Offset is an integer data type in MPI standard used 
   // for file sizes, displacements, and offsets in MPI I/O operations
   MPI_Offset offset = (MPI_Offset)mpi_rank * (MPI_Offset)n_local_elements * (MPI_Offset)double_size;
   MPI_File_seek(fh, offset, MPI_SEEK_SET);

   MPI_Status status;
   MPI_File_write(fh, local_data, n_local_elements, MPI_DOUBLE, &status);

   // Verifying the count is correct in the communication.
   int write_count = 0;
   MPI_Get_count(&status, MPI_DOUBLE, &write_count);
   if (write_count != n_local_elements)
   {
      printf("Rank %d: expected to write %d doubles, wrote %d\n",
              mpi_rank, n_local_elements, write_count);
   }

   MPI_File_close(&fh);

   if (mpi_rank == 0)
   {
      printf("Wrote %d doubles (%d per rank across %d ranks) to %s\n",
             n_global_elements, n_local_elements, num_of_ranks, filename);
   }

   free(local_data);
   MPI_Finalize();
   return 0;
}
