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

   char *filename = "nonuniform_array.bin";

   int n_local_elements = mpi_rank + 2;


   double *local_data = allocate_1d_double(&n_local_elements);
   for (int i = 0; i < n_local_elements; i++)
   {
      local_data[i] = (double)mpi_rank;
   }

   MPI_File fh;
   MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

   int double_size = 0;
   MPI_Type_size(MPI_DOUBLE, &double_size);
   int start_elem = 0;
   // Scan excluding the local mpi_rank value
   MPI_Exscan(&n_local_elements, &start_elem, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   // Carefull that first element is not defined.
   if (mpi_rank == 0)
   {
      start_elem = 0;
   }
   MPI_Offset offset = (MPI_Offset)start_elem * (MPI_Offset)double_size;
   MPI_File_seek(fh, offset, MPI_SEEK_SET);

   MPI_Status status;
   MPI_File_write(fh, local_data, n_local_elements, MPI_DOUBLE, &status);

   int write_count = 0;
   MPI_Get_count(&status, MPI_DOUBLE, &write_count);
   if (write_count != n_local_elements)
   {
      printf("mpi_rank %d: wrote %d of %d doubles\n", 
         mpi_rank, write_count, n_local_elements);
   }

   printf("mpi_rank %d: start_elem=%d, n_local_elements=%d, byte_offset=%lld\n",
          mpi_rank, start_elem, n_local_elements, (long long)offset);

   MPI_File_close(&fh);
   free(local_data);
   MPI_Finalize();
   return 0;
}