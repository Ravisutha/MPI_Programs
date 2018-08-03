/* * * * * * * * * * * * * * * * *
 * Program: MPI Basic Example 
 * * * * * * * * * * * * * * * * */ 

#include <stdio.h> 
#include <mpi.h> 

int main (int argc, char *argv[]) 
{ 
	int rank; /* Rank is your pid, staring with 0 */

	int size; /* size, is the number of processes you */ 
		  /* run the program with */

	/* never make MPI calls before this and 
	 * never touch argc and argv before doing this */ 
	MPI_Init (&argc, &argv);

	/* get current MPI task number */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 

	/* get number of MPI tasks */
	MPI_Comm_size (MPI_COMM_WORLD, &size); 

	/* regular old printf */
	printf("Hello world from task %d of %d\n", rank, size);

	/* don't make MPI calls after this */ 
	MPI_Finalize(); 

	return 0; 
} 
