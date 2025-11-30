#include <stdio.h>
#include <mpi.h>

#include "arrays.h"

int main(int argc, char* argv[])
{
   int num_of_ranks;
   int mpi_rank;

   double* vector;
   int number_of_elements;

   // Initialize the MPI environment
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   // First rank initializes the vector
   if (mpi_rank == 0)
   {
      number_of_elements = 5;
      vector = allocate_1d_double(number_of_elements);
      intialize_1d_double(vector, &number_of_elements);
   }
   /*
   1. Bcast number of elements of the vector
   2. Other ranks allocate the vector and initialize to 0
   3. Print vectors before bcasting
   */
   MPI_Bcast( &number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
   if (mpi_rank != 0)
      vector = allocate_1d_double(number_of_elements);
   print_1d_double(vector,&number_of_elements, &mpi_rank);
   sleep(1);

   /*
   1. Bcast number of elements of the vector
   2. Other ranks allocate the vector and initialize to 0
   3. Print vectors after bcasting
   */
   MPI_Bcast( vector, number_of_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   if (mpi_rank == 0)
     printf("-----------------------------------------------------------\n");
   print_1d_double(vector,&number_of_elements, &mpi_rank);

   // Finalize the MPI environment
   MPI_Finalize();

   return 0;
}
