#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../Libraries/mt19937ar.h"

// Probability to sample (notice that normalization is missing)
double probability_density(double x){
   double Pi=exp(-0.5*x*x);
   return Pi;
}

int main(int argc, char *argv[])
{
   int num_samplings = 100000;  // Number of samples
   double proposal_width = 0.1; // Step size
   double current_x =0.0;       // Initial position

   int accepted_moves = 0;      // Computing acceptance rate.

   // Seed the random number generator
   init_genrand(12345);

   printf("-------- Starting the simulation -------\n");
   printf("StepNumber, Position\n");
   printf("%d,%.7e\n", 0, current_x);

   for (int i = 1; i < num_samplings+1; i++)
   {
      // Propose a new position: x' within the intervale [-proposal_width:proposal_width] around x
      double proposed_x = current_x + proposal_width * (2.0*genrand_real1()-1.0) ;

      // Compute acceptance probability Pi(x')/Pi(x)
      double A = probability_density(proposed_x) / probability_density(current_x);

      // Accept or reject based on the Metropolis criterion
      if ( A >= 1.0 || A >= genrand_real1() )
      {
         current_x = proposed_x;
         accepted_moves++;
      }
      // Print random walk
      printf("%d,%.7e\n", i, current_x);
      
   }

   // Print diagnostic statistics to stderr so it doesn't mix with CSV output
   printf("----------- Simulation Summary ---------\n");
   printf("Total Steps: %d\n", num_samplings);
   printf("Acceptance Rate: %.2f%%\n", (double)accepted_moves / num_samplings * 100.0);

   return 0;
}