#include <stdio.h>
#include "../Libraries/arrays.h"
#include <mpi.h>

#define PARTICLE_STRUCT_VARIABLES 4

// Define the particle struct
typedef struct
{
    int id;
    double mass;
    double position[3];
    double velocity[3];
} Particle;

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    int num_of_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    int num_particles = 10;
    Particle *particles = (Particle *)malloc(num_particles * sizeof(Particle));

    MPI_Datatype MPI_variables_types[PARTICLE_STRUCT_VARIABLES] = {MPI_INT, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
    int blocklengths[PARTICLE_STRUCT_VARIABLES] = {1, 1, 3, 3};
    long int displacements[PARTICLE_STRUCT_VARIABLES];
    long int base_struct_address;

    Particle dummy_particle;
    MPI_Get_address(&dummy_particle, &base_struct_address);
    MPI_Get_address(&dummy_particle.id, &displacements[0]);
    MPI_Get_address(&dummy_particle.mass, &displacements[1]);
    MPI_Get_address(&dummy_particle.position[0], &displacements[2]);
    MPI_Get_address(&dummy_particle.velocity[0], &displacements[3]);
    for (int i = 0; i < PARTICLE_STRUCT_VARIABLES; i++)
    {
        displacements[i] -= base_struct_address;
    }

    MPI_Datatype MPI_RAW_PARTICLE_TYPE;
    MPI_Type_create_struct(PARTICLE_STRUCT_VARIABLES, &blocklengths[0], &displacements[0], &MPI_variables_types[0], &MPI_RAW_PARTICLE_TYPE);

    MPI_Datatype MPI_PARTICLE_TYPE;
    MPI_Type_create_resized(MPI_RAW_PARTICLE_TYPE, 0, sizeof(Particle), &MPI_PARTICLE_TYPE);

    MPI_Type_commit(&MPI_PARTICLE_TYPE);

    MPI_Type_free(&MPI_RAW_PARTICLE_TYPE);

    if (mpi_rank == 0)
    {
        init_genrand(100);
        for (int i = 0; i < num_particles; i++)
        {
            particles[i].id = i;
            particles[i].mass = 1.0;
            particles[i].position[0] = 2.0 * ((double)i) / ((double)num_particles) - 1.0;
            particles[i].position[1] = 2.0 * ((double)i) / ((double)num_particles) - 1.0;
            particles[i].position[2] = 2.0 * ((double)i) / ((double)num_particles) - 1.0;
            particles[i].velocity[0] = genrand_real1() - 2.0;
            particles[i].velocity[1] = genrand_real1() - 2.0;
            particles[i].velocity[2] = genrand_real1() - 2.0;
        }
    }
    else
    {
        for (int i = 0; i < num_particles; i++)
        {
            particles[i].id = 0;
            particles[i].mass = 0.0;
            particles[i].position[0] = 0.0;
            particles[i].position[1] = 0.0;
            particles[i].position[2] = 0.0;
            particles[i].velocity[0] = 0.0;
            particles[i].velocity[1] = 0.0;
            particles[i].velocity[2] = 0.0;
        }
    }

    MPI_Bcast(particles, num_particles, MPI_PARTICLE_TYPE, 0, MPI_COMM_WORLD);

    for (int i = 1; i < num_of_ranks; i++)
    {
        if (mpi_rank == i)
        {
            printf(" ======== Vector from rank %d \n", mpi_rank);
            for (int j = 0; j < num_particles; j++)
            {
                printf("Particle[%d] -> ID: %d, Mass: %.1f, Pos: (%.1f, %.1f, %.1f), Vel: (%.1f, %.1f, %.1f)\n",
                       j, particles[j].id, particles[j].mass, 
                       particles[j].position[0], particles[j].position[1], particles[j].position[2],
                       particles[j].velocity[0], particles[j].velocity[1], particles[j].velocity[2]);
            }
        }
    }

    free(particles);
    MPI_Type_free(&MPI_PARTICLE_TYPE);

    MPI_Finalize();
    return 0;
}