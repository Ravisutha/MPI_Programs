/* 
 * Author : Ravisutha Sakrepatna Srinivasamurthy
 * HW 	: Floyd's Algorithm
 * Class: Intro to Parallel Systems 
 * Program: Main Program which computes the shortest path
 */

#include "graph.h"
#include <mpi.h>
#include "floyd_seq.h"

int main (int argc, char *argv[])
{
	/* Declaration goes here */
	int option = 0, rows = 0, range1 = 0, range2 = 0, len = 0;

	int **array;

	char *filename1, *filename2;

	double start_time, end_time;

	/* If the command line argument is not supplied */
	if (argc < 3)
	{
		printf ("Input the data file and output file:%d\n", argc);
		return -1;
	}
	
	filename1 = argv[1];
	
	filename2 = argv[2];

	/* Read the file */
	if (read_file (filename1, &rows, &array) == -1)
	{
		printf ("ERROR: reading the file\n");

		return -1;
	}
	
	MPI_Init (&argc, &argv);
	
	/* Begin the timer */
	start_time = MPI_Wtime ();

	compute_shortest_path(rows, array);	

	/* End the timer */
	end_time = MPI_Wtime ();

	printf ("Time taken for the computing floyd's alg = %f\n", end_time - start_time);

	MPI_Finalize ();

	printf ("Writing the output to %s\n", filename2);

	/* Write the output to the given file */
	if (write_graph_file (filename2, rows, array) == -1)
	{
		printf ("ERROR: writing to a file\n");

		return -1;
	}

	printf ("Write successful\n");

	return 0;
}

