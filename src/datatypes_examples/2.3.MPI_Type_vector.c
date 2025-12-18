#include <mpi.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
  int myid, npes, size, tag = 666, i;
	MPI_Status status;
        float received[9];
	float iarr[18];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);       
	  
	MPI_Datatype positions;
	MPI_Type_vector(3, 3, 6,MPI_FLOAT, &positions);
	MPI_Type_commit(&positions);

	if(myid == 0){

	  for (i = 0; i < 18; i++){
	    iarr[i]=(float)(i);}
	  
	  MPI_Send(&iarr[0], 1 ,positions, 1, tag, MPI_COMM_WORLD);}
	
	else 
	   MPI_Recv(&received, 9, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);

	MPI_Type_free(&positions);

	if (myid != 0){
	  printf("I am the node %d. Matrix elements received \n", myid);
	  for (i = 0; i < 9; i++)
	    printf("Vector element: %f \n", received[i]);
	}

	MPI_Finalize();
}
