#include <mpi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Libraries/arrays.h"

#define PARTICLE_STRUCT_VARIABLES 4

typedef struct
{
   long long id;
   double mass;
   double position[3];
   double velocity[3];
} Particle;

static MPI_Datatype Create_MPI_PARTICLE(void)
{
   MPI_Datatype MPI_variables_types[PARTICLE_STRUCT_VARIABLES] = {MPI_LONG_LONG, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
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
   MPI_Type_free(&MPI_RAW_PARTICLE_TYPE);

   MPI_Type_commit(&MPI_PARTICLE_TYPE);

   return MPI_PARTICLE_TYPE;
}

int main(int argc, char *argv[])
{
   MPI_Init(NULL, NULL);
   int num_of_ranks;
   MPI_Comm_size(MPI_COMM_WORLD, &num_of_ranks);
   int mpi_rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

   char *filename = "particles.bin";
   int n_local_particles = mpi_rank + 2;

   int start_index = 0;
   MPI_Exscan(&n_local_particles, &start_index, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   if (mpi_rank == 0)
   {
      start_index = 0;
   }

   Particle *particles = (Particle *)malloc((size_t)n_local_particles * sizeof(Particle));

   MPI_Datatype MPI_PARTICLE = Create_MPI_PARTICLE();

   MPI_File fh;
   MPI_File_open(MPI_COMM_WORLD, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

   MPI_Aint lb = 0, extent = 0;
   MPI_Type_get_extent(MPI_PARTICLE, &lb, &extent);
   MPI_Offset offset = (MPI_Offset)start_index * (MPI_Offset)extent;

   MPI_Status status;
   MPI_File_read_at_all(fh, offset, particles, n_local_particles, MPI_PARTICLE, &status);

   int read_count = 0;
   MPI_Get_count(&status, MPI_PARTICLE, &read_count);
   if (read_count != n_local_particles)
   {
      printf("Rank %d: read %d of %d particles\n", mpi_rank, read_count, n_local_particles);
   }

   for (int i = 0; i < read_count; ++i)
   {
      printf("Rank %d | id=%lld mass=%.2f pos=(%.1f, %.1f, %.1f) vel=(%.2f, %.2f, %.2f)\n",
             mpi_rank,
             particles[i].id,
             particles[i].mass,
             particles[i].position[0], particles[i].position[1], particles[i].position[2],
             particles[i].velocity[0], particles[i].velocity[1], particles[i].velocity[2]);
   }

   MPI_File_close(&fh);
   MPI_Type_free(&MPI_PARTICLE);
   free(particles);
   MPI_Finalize();
   return 0;
}