#include <mpi.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
  int myid, npes, size, tag = 666, i;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npes);	
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);       
  int received[4];
	  
  MPI_Datatype column;
  MPI_Type_vector(4,1,4,MPI_INT, &column);
  MPI_Type_commit(&column);

  if(myid == 0){
    int array44[4][4] = {
			 {32, 33, 34, 35},
			 {25, 26, 27, 28},
			 {18, 19, 20, 21},
			 {11, 12, 13, 14},
    };
    for (i=0; i<npes; i++){
      MPI_Send(&array44[0][i], 1 ,column, i, tag, MPI_COMM_WORLD);
    }
  }
	
 
  MPI_Recv(received, 4, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);

  MPI_Barrier(MPI_COMM_WORLD);

  
  printf("I am the node %d. Matrix elements received %d,%d,%d,%d \n", myid, received[0],received[1], received[2], received[3]);
  

  MPI_Finalize();
}
