#include <mpi.h>
#include <stdio.h>
#include <string.h>


void matrix_initial(int N, float matrix[N][N]){
  int value=0;
  for (int i = 0; i < N; i++) {
     for (int j = 0; j <  N; j++) {
        matrix[i][j] = value;
	value++;
     }
  }
}

void matrix_set_zero(int N, float matrix[N][N]){
  for (int i = 0; i < N; i++) {
     for (int j = 0; j <  N; j++) {
        matrix[i][j] = 0;
     }
  }
}

void print_matrix(int N,  float matrix[N][N]){
  
    for (int i = 0; i < N ; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
  int myid, npes, size, tag = 666, i, j;
  float mat44[4][4], tmat_recv[10], value=0.0;
  int blocklength[4], displ[4];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npes);	
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);


/* Define the datatype, elements per block and displacement */
    MPI_Datatype trilow;
    value=0;

    for (i=0;i<4;i++){
      blocklength[i]=i+1;
      displ[i]=4*i;
      /* printf("I am process %d, and this is the element %d. Blocklength: %d. Displacement: %d \n", myid, i, blocklength[i], displ[i]); */

    }
	
    MPI_Type_indexed(4, blocklength, displ, MPI_FLOAT,&trilow);
    MPI_Type_commit(&trilow);
 
  /* Define the matrix that we are going to work with */

  

  if (myid==0){
    matrix_initial(4, mat44);
    printf("I am process %d, and this is the whole matrix:\n",myid);
    print_matrix(4,mat44);     
    MPI_Send(&(mat44[0][0]), 1, trilow, 1, tag, MPI_COMM_WORLD);
  

 }
  else if (myid==1){
    matrix_set_zero(4,mat44);
    MPI_Recv(&(mat44[0][0]), 1, trilow, 0, tag, MPI_COMM_WORLD, &status);
    printf("I am process %d, and this is the  matrix received: \n", myid);
    print_matrix(4,mat44);
	
  }

  
  MPI_Finalize();

}
