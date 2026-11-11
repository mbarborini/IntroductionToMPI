#include <math.h>
#include <stdio.h>
#include "../Libraries/arrays.h"

#define N_GRID_POINTS 1024          // Grid interior points
#define N_TEMP_STEPS 10      // Number of time steps
#define D 0.01          // Thermal diffusion constant
#define L 1.0           // Length of the 1D bar

int main() {

    double dx = 1.0 / (N_GRID_POINTS + 1);
    // Choose dt to satisfy stability criterion for explicit Euler: r = D * dt / dx^2 <= 0.5
    double dt = 0.4 * (dx * dx) / D; 
    double r = D * dt / (dx * dx);
    printf("Grid points: %d, Time steps: %d, dx: %e, dt: %e, r: %f\n", N_GRID_POINTS, N_TEMP_STEPS, dx, dt, r);

    int num_elements = N + 2;
    double *u     = allocate_1d_double(&num_elements);
    double *u_new = allocate_1d_double(&num_elements);

    // Enforce Dirichlet boundary conditions (u = 0 at ends)
    u[0]     = 0.0;
    u[N + 1] = 0.0;
    for (int i = 1; i <= N_GRID_POINTS; i++) {
        double x = i * dx;
        u[i] = sin(M_PI * x);
    }

    // 2. Time-Stepping Loop
    for (int step = 0; step < N_TEMP_STEPS; step++) {

        // --- Explicit Finite Difference Update ---
        u_new[0]     = 0.0;
        u_new[N + 1] = 0.0;
        for (int i = 1; i <= N_GRID_POINTS; i++) {
            u_new[i] = u[i] + r * (u[i - 1] - 2.0 * u[i] + u[i + 1]);
        }
        
        u = u_new;
    }

    printf("Simulation completed successfully after %d time steps.\n", N_TEMP_STEPS);

    return 0;
}