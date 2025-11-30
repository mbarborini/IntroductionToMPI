#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
   int num_of_ranks;
   int mpi_rank;

   double variable;

   // Initialize the MPI environment
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   // Initializes the variable's value
   if (mpi_rank == 0){
      variable = 4.0;
   }else{
      variable = 0.0;
   }

   // Print variables before bcasting
   printf("Value of the variable for rank %d out of %d processes : %f \n", mpi_rank, num_of_ranks, variable);
   sleep(1);
   
   // Bcation variable
   MPI_Bcast( &variable, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

   // Print variables before bcasting
   if (mpi_rank == 0) 
      printf("-----------------------------------------------------------\n");
   printf("Value of the variable for rank %d out of %d processes : %f \n", mpi_rank, num_of_ranks, variable);

   // Finalize the MPI environment
   MPI_Finalize();

   return 0;
}
