#include <stdlib.h>
#include "mt19937ar.h"

// Allocate empty matrix
double **allocate_2d_double(int *n, int *m)
{
   if (*n <= 0 || *m <= 0)
      return NULL;

   // Allocate memory for the row pointers (an array of double*)
   double **matrix = (double **)malloc((double)*n * sizeof(double *));

   // Allocate memory for the elements of each row (and initialize to 0)
   for (int i = 0; i < *n; i++)
   {
      matrix[i] = (double *)calloc(*m, sizeof(double));
   }
   return matrix;
}

// Allocate packed empty matrix
double **allocate_2d_packed_double(int *n, int *m)
{
   if (*n <= 0 || *m <= 0)
      return NULL;

   // Allocate the rows*columns contiguous doubles
   double *temp_matrix = (double *)malloc(*n * *m * sizeof(double));

   // Allocate memory for the row pointers (an array of double*)
   double **matrix = (double **)malloc(*n * sizeof(double *));

   // Associated to each row the consecutive pointers previously initialized
   for (int i = 0; i < *n; i++)
   {
      matrix[i] = &(temp_matrix[i * *m]);
   }

   return matrix;
}

// Allocate empty vector
double *allocate_1d_double(int *n)
{
   if (*n <= 0)
      return NULL;

   // Allocate memory for the row pointers (an array of double*)
   double *vector = (double *)calloc(*n, sizeof(double));
   return vector;
}

// Allocate empty vector
int *allocate_1d_int(int *n)
{
   if (*n <= 0)
      return NULL;

   // Allocate memory for the row pointers (an array of int*)
   int *vector = (int *)calloc(*n, sizeof(int));
   return vector;
}

// Print matrix
void print_2d_double(int *n, int *m, double **A)
{
   printf("Number of Rows and Columns: %4i, %4i\n", *n, *m);
   printf("Elements (Column first):\n");

   for (int i = 0; i < *n; i++)
   {
      for (int j = 0; j < *m; j++)
      {
         printf(" %4.2f ", A[i][j]);
      }
      printf("\n");
   }
}

// Print vector
void print_1d_double(int *n, double *a)
{
   printf("Number of elements: %4i \n", *n);
   printf("Elements :\n");

   for (int i = 0; i < *n; i++)
   {
      printf(" %4.2f ", a[i]);
   }
   printf("\n");
}

// Print vector
void print_1d_int(int *n, int *a)
{
   printf("Number of elements: %4i \n", *n);
   printf("Elements :\n");

   for (int i = 0; i < *n; i++)
   {
      printf(" %d ", a[i]);
   }
   printf("\n");
}


// Initialize a random matrix  with elements from [-1:1]
void init_random_matrix(int seed,int *n, int *m, double **A)
{
   if (seed > 0){
      init_genrand(seed); 
   }
   for (int i = 0; i < *n; i++)
   {
      for (int j = 0; j < *m; j++)
      {
         A[i][j] = 2.0*genrand_real1()-1.0;
      }
   }
}

// Initialize a random vector  with elements from [-1:1]
void init_random_vector(int seed, int *n, double *a)
{
   if (seed > 0){
      init_genrand(seed); 
   } 
   for (int i = 0; i < *n; i++)
   {
      a[i] = 2.0*genrand_real1()-1.0;
   }
}