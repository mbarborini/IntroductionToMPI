#include <math.h>
#include <stdio.h>
#include "../Libraries/arrays.h"

#define N_GRID_POINTSX 512 // Grid interior points along x
#define N_GRID_POINTSY 512 // Grid interior points along y
#define N_TEMP_STEPS 100   // Number of time steps
#define D 1.0              // Thermal diffusion constant
#define L 1.0              // Length of the side of the plane

int main()
{

    double dx = L / (N_GRID_POINTSX + 1);
    double dy = L / (N_GRID_POINTSY + 1);
    // Choose dt to satisfy stability criterion for explicit Euler: r = D * dt / dx^2 <= 0.5
    double dt = 0.4 / D * ( (dx * dx) + (dy * dy) );
    double rx = D * dt / (dx * dx);
    double ry = D * dt / (dy * dy);

    int num_elementsx = N_GRID_POINTSX + 2;
    int num_elementsy = N_GRID_POINTSY + 2;
    double **u = allocate_2d_packed_double(&num_elementsx, &num_elementsy);
    double **u_new = allocate_2d_packed_double(&num_elementsx, &num_elementsy);

    // Initial Conditions: Create a hot square in the center
    for (int i = 1; i <= N_GRID_POINTSX; i++) {
        double x = i * dx;
        for (int j = 1; j <= N_GRID_POINTSY; j++) {
            double y = j * dy;
            u[i][j] =sin(M_PI * x)*sin(M_PI * y); 
        }
    }
    for (int i = 0; i <= N_GRID_POINTSY+1; i++) {
        u[0][i] = 0.0; u[N_GRID_POINTSX+1][i] = 0.0;
    }
    for (int i = 0; i <= N_GRID_POINTSX+1; i++) {
        u[i][0] = 0.0; u[i][N_GRID_POINTSY+1] = 0.0;
    }

    for (int step = 1; step <= N_TEMP_STEPS; step++)
    {
        // Update interior grid points
        for (int i = 1; i <= N_GRID_POINTSX; i++) {
            for (int j = 1; j <= N_GRID_POINTSY; j++) {
                u_new[i][j] = u[i][j] +
                    rx * (u[i+1][j] - 2.0 * u[i][j] + u[i-1][j]) +
                    ry * (u[i][j+1] - 2.0 * u[i][j] + u[i][j-1]);
            }
        }
        
        double **temp = u; 
        u = u_new; 
        u_new = temp;

    }

    return 0;
}



