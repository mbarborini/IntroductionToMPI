#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define tambuff 100

int main(int argc, char *argv[])
{
	int myid, npes, size;
        
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	char packbuff[tambuff];
	int el1;
	float el2;
	if (myid == 0){ 
	  el1=250;
	  el2=255.5;
	  int position=0;

	  // Pack integer
	  MPI_Pack(&el1, 1, MPI_INT, packbuff, tambuff, &position, MPI_COMM_WORLD);

	  // Pack float
	  MPI_Pack(&el2, 1, MPI_FLOAT, packbuff, tambuff, &position, MPI_COMM_WORLD);

	}
	  MPI_Bcast(&packbuff, tambuff, MPI_PACKED, 0, MPI_COMM_WORLD);
	
	if (myid!=0)
	  {
	    int recv_position=0;

	    MPI_Unpack(packbuff,tambuff,&recv_position,&el1,1,MPI_INT,MPI_COMM_WORLD);
	    MPI_Unpack(packbuff,tambuff,&recv_position,&el2,1,MPI_FLOAT,MPI_COMM_WORLD);
	    
	    printf("I am the node %d and a is %d and b is %f\n", myid, el1, el2); 

	  }
	
	MPI_Finalize();
}
