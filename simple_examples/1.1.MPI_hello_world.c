#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

   // Initialize the MPI environment
   MPI_Init(&argc, &argv);

   // Get the number of processes
   int num_of_ranks;
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);

   // Get the rank of the process
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   // Get the name of the processor (Not commonly used)
   printf("Maximum length of processor name : %d\n", MPI_MAX_PROCESSOR_NAME);
   char processor_name[MPI_MAX_PROCESSOR_NAME];
   int name_len;
   MPI_Get_processor_name(processor_name, &name_len);

   // Print hello world from all MPI tasks
   printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, mpi_rank, num_of_ranks);

   // Finalize the MPI environment
   MPI_Finalize();

   return 0;
}
