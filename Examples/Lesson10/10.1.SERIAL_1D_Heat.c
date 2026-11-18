#include <math.h>
#include <stdio.h>
#include "../Libraries/arrays.h"

#define N_GRID_POINTS 2048 // Grid interior points
#define N_TEMP_STEPS 10000000  // Number of time steps
#define D 0.1             // Thermal diffusion constant
#define L 1.0              // Length of the 1D bar

int main()
{
    double dx = L / (N_GRID_POINTS + 1);
    // Choose dt to satisfy stability criterion for explicit Euler: r = D * dt / dx^2 <= 0.5
    double dt = 0.4 * (dx * dx) / D;
    double r = D * dt / (dx * dx);

    int num_elements = N_GRID_POINTS + 2;
    double *u = allocate_1d_double(&num_elements);
    double *u_new = allocate_1d_double(&num_elements);

    // Enforce Dirichlet boundary conditions (u = 0 at ends)
    u[0] = 0.0;
    u[N_GRID_POINTS + 1] = 0.0;
    for (int i = 1; i <= N_GRID_POINTS; i++)
    {
        u[i] = sin(M_PI * (double)i * dx / L);
    }
    for (int i = 0; i < N_GRID_POINTS + 2; i++)
    {
        printf(" %10.4f \n", u[i]);
    }
    printf("\n");

    for (int step = 1; step <= N_TEMP_STEPS; step++)
    {

        u_new[0] = 0.0;
        u_new[N + 1] = 0.0;
        for (int i = 1; i <= N_GRID_POINTS; i++)
        {
            u_new[i] = u[i] + r * (u[i - 1] - 2.0 * u[i] + u[i + 1]);
        }
        
        double *temp = u; 
        u = u_new; 
        u_new = temp;

        if (step % 1000000 == 0)
        {
            for (int i = 0; i < N_GRID_POINTS + 2; i++)
            {
                printf(" %10.4f \n", u[i]);
            }
            printf("\n");
        }
    }

    return 0;
}