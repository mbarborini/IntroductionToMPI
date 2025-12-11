#include <mpi.h>
#include <stdio.h>
#include <string.h>

void print_matrix(int N,  int matrix[N*N]){
  for (int i = 0; i < N ; ++i) {
    for (int j = 0; j < N; ++j) {
      printf("%d ", matrix[i*N+j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  int myid, npes, size, tag = 666, i, j;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npes);	
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  
  int received[4];
  int array44[4][4];

  if(myid == 0){
    int tmp[4][4] = {
		     {32, 33, 34, 35},
		     {25, 26, 27, 28},
		     {18, 19, 20, 21},
		     {11, 12, 13, 14},
    };
    
    for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
	array44[i][j] = tmp[i][j];
      }
    }
  }
  
  MPI_Datatype tile, resized_tile;
  int array_dimensions = 2;
  int local_dim = 2;
  int array_element_counts[2] = {4, 4}; //size of global array
  int subarray_element_counts[2] = {local_dim, local_dim}; //size of matrix tiles
  int subarray_coordinates[2] = {0, 0}; // start region
  
  MPI_Type_create_subarray(array_dimensions, array_element_counts, subarray_element_counts, subarray_coordinates, MPI_ORDER_C, MPI_INT, &tile);
  MPI_Type_create_resized(tile, 0, local_dim*sizeof(int), &resized_tile);
  MPI_Type_commit(&resized_tile);

  int counts[4] = {1, 1, 1, 1};
  int displs[4] = {0, 1, 4, 5};

  MPI_Scatterv(array44, counts, displs, resized_tile, received, local_dim*local_dim, MPI_INT, 0, MPI_COMM_WORLD);
  printf("I am the node %d\n", myid);
  print_matrix(2, received);

  MPI_Finalize();
}
