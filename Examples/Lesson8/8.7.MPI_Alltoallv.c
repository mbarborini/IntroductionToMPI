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

   int *size_send_blocks = allocate_1d_int(&num_of_ranks);
   init_genrand(10+mpi_rank);
   for (int i = 0; i < num_of_ranks; i++)
   {
      size_send_blocks[i] = (int)(1+genrand_real1() * 4);
   }

   int *send_displacements = allocate_1d_int(&num_of_ranks); 
   send_displacements[0]= 0;
   for (int i = 1; i <= num_of_ranks; i++)
   {
      send_displacements[i] = send_displacements[i-1]+size_send_blocks[i-1];
   }

   int total_send_size = send_displacements[num_of_ranks-1]+size_send_blocks[num_of_ranks-1];
   double *send_buffer = allocate_1d_double(&total_send_size);

   int k = 0;
   for (int i = 0; i < num_of_ranks; i++)
   {
      for (int j = 0; j < size_send_blocks[i]; j++)
      {
         send_buffer[k] = (double) i+1+mpi_rank*10;
         k++;
      }
   }

   int *size_receive_blocks = allocate_1d_int(&num_of_ranks);

   MPI_Alltoall(size_send_blocks, 1, MPI_INT, size_receive_blocks, 1, MPI_INT, MPI_COMM_WORLD);

   int  *receive_displacements = allocate_1d_int(&num_of_ranks);
   receive_displacements[0] = 0;
   for (int i = 1; i <= num_of_ranks; i++)
   {
      receive_displacements[i] = receive_displacements[i-1] + size_receive_blocks[i-1];
   }

   int total_receive_size = receive_displacements[num_of_ranks-1] + size_receive_blocks[num_of_ranks-1];
   double *receive_buffer = allocate_1d_double(&total_receive_size);

   MPI_Alltoallv(&send_buffer[0], &size_send_blocks[0], &send_displacements[0], MPI_DOUBLE,
                 &receive_buffer[0], &size_receive_blocks[0], &receive_displacements[0], MPI_DOUBLE, MPI_COMM_WORLD);

   for (int i = 0; i < num_of_ranks; i++)
   {
      if(i == mpi_rank){
         printf("mpi_rank : %d\n", mpi_rank);
         printf("Total number of elements to send : %d\n",total_send_size);
         print_1d_double(&total_send_size, send_buffer);
         printf("Total number of elements to receive : %d\n",total_receive_size);
         print_1d_double(&total_receive_size, receive_buffer);
      }
      MPI_Barrier(MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}