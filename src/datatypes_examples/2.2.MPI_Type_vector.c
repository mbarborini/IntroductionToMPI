#include <mpi.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
  int myid, npes, size, tag = 666, i, j;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);       
        int received[4];

	MPI_Datatype tile;
	MPI_Type_vector(2,2,4,MPI_INT, &tile);
	MPI_Type_commit(&tile);

	if(myid == 0){
	  int array44[4][4] = {
	   {32, 33, 34, 35},
	   {25, 26, 27, 28},
	   {18, 19, 20, 21},
	   {11, 12, 13, 14},
	};
	  for (i=0; i<npes/2; i++){
	    for (j=0; j<npes/2; j++){
	      MPI_Send(&array44[2*i][2*j], 1 ,tile, (2*i+j), tag, MPI_COMM_WORLD);
	    }
	  }
	}
	
        MPI_Recv(received, 4, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);

        MPI_Barrier(MPI_COMM_WORLD);

	
	printf("I am the node %d. Matrix elements received %d,%d,%d,%d \n", myid, received[0],received[1], received[2], received[3]);
	

	MPI_Finalize();
}
