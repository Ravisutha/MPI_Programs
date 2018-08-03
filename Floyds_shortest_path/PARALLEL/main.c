/* 
 * Program: Floyd's Parallel Execution
 * Author : Ravisutha Sakrepatna Srinivasamurthy
 * Class  : Intro to Parallel Systems
 */ 

#include "MyMPI.h"

#define dtype MPI_INT;

int main (int argc, char *argv[]) 
{ 
	/* Declarations goes here */
	int rank; /* rank is your pid, staring with 0 */

	int size; /* size: number of processes*/ 

	MPI_Comm cart_comm, row_comm, col_comm;

	int cart_size[2] = {0, 0};

	int periodic[2] = {0, 0};

	int m, n, id, i;

	int **subs, *storage;

	int *array;

	int grid_period[2], grid_coords[2], grid_size[2];

	int id1;

	double start_time1, start_time2,end_time;

	double time_compute, time_overall, avg_time1, avg_time2, avg_compute;

	/* Start Parallel Program */
	MPI_Init (&argc, &argv);

	/* Record initial time */
	start_time1 = MPI_Wtime();

	/* Get necessary details  */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
	MPI_Comm_size (MPI_COMM_WORLD, &size); 

	/* Check if command line argument is passed or not */
	if (argc < 1)
	{
		printf ("-------Please input a file name-------\n");
		return -1;
	}

	/* Square dimension */
	MPI_Dims_create(size, 2, cart_size);

	/* Create a cartesian communicator */
	MPI_Cart_create (MPI_COMM_WORLD, 2, cart_size, periodic, 0, &cart_comm);

	if (cart_comm == NULL)
	{
		printf ("Cart comm not created, exiting\n");
		return -1;
	}

	/* Read the matrix and distribute among the process */
	read_checkerboard_matrix(argv[1], (void ***) &subs, (void **)&storage, MPI_INT, &m, &n, cart_comm);      /* IN - Communicator */

	/* Create row and column communicator */
	MPI_Comm_rank (cart_comm, &id); 
	MPI_Cart_get(cart_comm, 2, grid_size, grid_period, grid_coords);

	/* Create column and row communicators */
	MPI_Comm_split (cart_comm, grid_coords[1], grid_coords[0], &col_comm);
	MPI_Comm_split (cart_comm, grid_coords[0], grid_coords[1], &row_comm);

	if (id == 0)
	{
		printf ("Matrix size is %d*%d\n", m, n);
	}

	MPI_Barrier (MPI_COMM_WORLD);

	start_time2 = MPI_Wtime();

	/* Create column communicator */
	compute_shortest_path (m, subs, cart_comm, grid_size, row_comm, col_comm);

	MPI_Barrier (MPI_COMM_WORLD);

	/* Record end time */
	end_time = MPI_Wtime();

	time_compute = end_time - start_time2;

	MPI_Reduce(&time_compute, &avg_time1, 1, MPI_DOUBLE, MPI_SUM, 0, cart_comm);

	/* Calculate the avg time */
	if (id == 0)
	{
		printf ("Time taken to compute = %f\n", avg_time1 / (float)size);
	}

	time_overall = end_time - start_time1;

	MPI_Reduce(&time_overall, &avg_time2, 1, MPI_DOUBLE, MPI_SUM, 0, cart_comm);
	
	/* Calculate the avg time */
	if (id == 0)
	{
		printf ("Time taken overall = %f\n", avg_time2 / (float)size);
	}

	print_checkerboard_matrix((void **)subs, MPI_INT, m, n, cart_comm, argv[2]);

	MPI_Finalize(); 

	return 0; 
} 
