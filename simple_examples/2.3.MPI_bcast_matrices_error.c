#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#include "arrays.h"

int main(int argc, char* argv[])
{
   int num_of_ranks;
   int mpi_rank;

   double** matrix;
   int number_of_rows, number_of_columns;

   // Initialize the MPI environment
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   // First rank initializes 
   if (mpi_rank == 0)
   {
      number_of_rows = 2;
      number_of_columns = 2;
      matrix = allocate_2d_double(number_of_rows, number_of_columns);
      intialize_2d_double(matrix, &number_of_rows, &number_of_columns );
   }

   MPI_Bcast(&number_of_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&number_of_columns, 1, MPI_INT, 0, MPI_COMM_WORLD);
   if (mpi_rank != 0)
      matrix = allocate_2d_double(number_of_rows, number_of_columns);

   
   if (mpi_rank == 0)
      printf("-----------------------------------------------------------\n");
   print_2d_double(matrix, &number_of_rows, &number_of_columns, &mpi_rank);
   sleep(1);

   // Bcast matrix assumed to have consecutive memory 
   MPI_Bcast(&matrix[0][0], number_of_columns*number_of_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   if (mpi_rank == 0)
      printf("-----------------------------------------------------------\n");
   print_2d_double(matrix, &number_of_rows, &number_of_columns, &mpi_rank);

   // Finalize the MPI environment
   MPI_Finalize();

   return 0;
}
