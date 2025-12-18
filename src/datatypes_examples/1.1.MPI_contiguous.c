#include <mpi.h>
#include <stdio.h>
#include <string.h>



int main(int argc, char *argv[])
{
  int myid, npes, size, tag = 666, i, j;
	float array44[4][4];
	float vect_recv[4];
	
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	/* Create 4x4 array on rank 0 */
	if(myid==0) {
		for (i = 0; i < 4; i++)
		  for (j=0; j<4; j++)
		    array44[i][j] = (float)10*(i+j*0.1);
	}

	/* Create dataype. define typemap and commit  */
	MPI_Datatype vect_send;
	MPI_Type_contiguous(4, MPI_FLOAT, &vect_send);
	MPI_Type_commit(&vect_send);

	/* Send rows from process 0 to the other processes */
	if(myid == 0){
	  for (i=0;i<npes;i++)
	      MPI_Send(&array44[i][0], 1, vect_send, i, tag, MPI_COMM_WORLD);
	}
	
	MPI_Recv(vect_recv, 1, vect_send, 0, tag, MPI_COMM_WORLD, &status);

	MPI_Barrier(MPI_COMM_WORLD);


	/* Free vector data */
	MPI_Type_free(&vect_send);


	for (i = 0; i < 4; i++)
	  printf("I am the node %d. I have received element %d, which is %f \n", myid, i,vect_recv[i]);
	

	MPI_Finalize();
}
