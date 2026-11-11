#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "../Libraries/arrays.h"

#define N_GRID_POINTS 1024          // Grid interior points
#define N_TEMP_STEPS 10      // Number of time steps
#define D 0.01        // Thermal diffusion constant
#define L 1.0         // Length of the 1D bar

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int num_of_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    double dx = 1.0 / (num_of_ranks*N_GRID_POINTS + 1);
    // Choose dt to satisfy stability criterion for explicit Euler: r = D * dt / dx^2 <= 0.5
    double dt = 0.4 * (dx * dx) / D; 
    double r = D * dt / (dx * dx);    
    if (mpi_rank == 0) {
        printf("Grid points: %d, Time steps: %d, dx: %e, dt: %e, r: %f\n", num_of_ranks*N_GRID_POINTS, N_TEMP_STEPS, dx, dt, r);
    }

    // Setup 1D Cartesian Topology
    int dims[1] = {num_of_ranks};
    int periods[1] = {0}; // Non-periodic (Dirichlet boundaries: u = 0 at ends)
    int reorder = 1;
    MPI_Comm MPI_COMM_1D;
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, reorder, &MPI_COMM_1D);

    // Find left and right neighbors for each rank
    int left, right;
    MPI_Cart_shift(MPI_COMM_1D, 0, 1, &left, &right);

    // 2. Local Grid Setup & Parameters
    int num_local = N_GRID_POINTS;
    int num_elements = num_local + 2; // Adding local Boundary
    double *u     = allocate_1d_double(&num_elements);
    double *u_new = allocate_1d_double(&num_elements);
    
    // Initialize initial condition: u(x, 0) = sin(pi * x)
    u[0]     = 0.0;
    u[N + 1] = 0.0;
    for (int i = 1; i <= num_local; i++) {
        int global_i = mpi_rank * num_local + (i - 1);
        double x = (global_i + 1) * dx;
        u[i] = sin(M_PI * x);
    }

    // Time-evolution Loop
    for (int step = 0; step < N_TEMP_STEPS; step++) {

        // Send left boundary to LEFT neighbor, receive into right ghost cell from RIGHT neighbor
        MPI_Sendrecv(&u[1], 1, MPI_DOUBLE, left, 0,
                     &u[num_local + 1], 1, MPI_DOUBLE, right, 0,
                     MPI_COMM_1D, MPI_STATUS_IGNORE);

        // Send right boundary to RIGHT neighbor, receive into left ghost cell from LEFT neighbor
        MPI_Sendrecv(&u[num_local], 1, MPI_DOUBLE, right, 1,
                     &u[0], 1, MPI_DOUBLE, left, 1,
                     MPI_COMM_1D, MPI_STATUS_IGNORE);

        // Computing local Finite Difference Update  
        for (int i = 1; i <= num_local; i++) {
            u_new[i] = u[i] + r * (u[i - 1] - 2.0 * u[i] + u[i + 1]);
        }

        u = u_new;
    }

    if (mpi_rank == 0) {
        printf("Simulation completed successfully after %d time steps.\n", N_TEMP_STEPS);
    }

    MPI_Comm_free(&MPI_COMM_1D);
    MPI_Finalize();
    return 0;
}