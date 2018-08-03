/* * * * * * * * * * * * * * * * * * * * * * * * *
 * Assignment number : 02
 * Author: Ravisutha Sakrepatna Srinivasamurthy
 * Class: Parallel Systems (Dr. Ligon)
 * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	//Declarations goes here
	int rank, my_num, next_rank, pre_rank, size, k = 0, partial_sum = 0, temp = 0;

	//MPI initialization
	MPI_Init (&argc, &argv);

	//Get the rank of the process
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	//Get the number of process in MPI
	MPI_Comm_size (MPI_COMM_WORLD, &size);

	//Seed the random generator with rank of the process
	srand (rank);

	//Get the value of my_num between 0 to 100
	my_num = rand () % 100;

	//Initialize while loop
	k = 1;

	partial_sum = my_num;

	//Exchange the mynum with next and previous process and generate the sum
	// Using bit shifting operator to determine 2 powers	
	while (size >= (1 << k))
	{
		//Find the rank of the next process
		next_rank = (rank + (1 << (k - 1))) % size;

		//Find the rank of the previous process
		pre_rank = (rank + size - (1 << (k - 1))) % size;

		MPI_Send (&partial_sum, 1, MPI_INT, next_rank, 99, MPI_COMM_WORLD);

		MPI_Recv (&temp, 1, MPI_INT, pre_rank, 99, MPI_COMM_WORLD, NULL);

		partial_sum += temp;

		k++;
	}

	printf ("Rank[%d] has my_num = %d, sum = %d\n", rank, my_num, partial_sum);

	//End MPI
	MPI_Finalize ();

	return 0;
}
