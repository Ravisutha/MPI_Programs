/* 
 * Program: Gaussian Elimination Parallel Execution
 * Author : Ravisutha Sakrepatna Srinivasamurthy
 * Class  : Intro to Parallel Systems
 */ 

#include <stdio.h> 
#include <mpi.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{ 
	int rank, size, i, j, k;
	int rows, cols, rem_rows;
	int total_rows;
	double **array, **A, *x, *b, *b_original;
	double starttime, endtime, avgtime;
	FILE *fp;

	MPI_Init (&argc, &argv);

	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	/* Cyclic Decomposition */

	/* Number of rows and columns */
	if (get_rows_cols (&rows, &cols, argv[1]) == -1)
	{
		printf ("Enter a valid file name \n");
		MPI_Finalize ();
		return 0;
	}

	if (rows != cols)
	{
		printf ("Not possible to apply guassian elimination\n");
		MPI_Finalize ();
		return 0;
	}

	/* Calculate number of rows belonging to this process */
	rem_rows = rows % size;

	if (rank < rem_rows)
	{
		total_rows = (rows / size) + 1;
	}

	else
	{
		total_rows = rows / size;
	}

	//printf ("rank = %d, total_rows = %d\n", rank, total_rows);

	/* Get the rows belonging to this process*/
	if ((fp = fopen (argv[1], "r")) == NULL)
	{
		printf ("File doesn't exists\n");
		return -1;
	}

	array = malloc (total_rows * sizeof (double *)) ;


	/* Skip row and column information */
	fseek(fp, 2*sizeof (int), SEEK_SET);	

	/* Skip rows belonging to other processes */
	fseek(fp, rank * (rows + 1) * sizeof (double), SEEK_CUR);	

	for (i = 0; i < total_rows; i++)
	{
		if (i != 0)
		{
			fseek(fp, (size - 1) * (rows + 1) * sizeof (double), SEEK_CUR);	
		}

		array[i] = malloc ((cols + 1)*sizeof(double));

		/* Fill the array */
		for (j = 0; j <= cols; j++)
		{
			fread(&(array[i][j]), sizeof (double), 1, fp);
			//printf ("%d: %lf    ", rank, array[i][j]);
		}
		//printf ("\n");
	}
	
	starttime = MPI_Wtime();
	guassian_elimination (total_rows, cols, rank, size, array);
	endtime = MPI_Wtime();

	endtime = endtime - starttime;

	MPI_Reduce (&endtime, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

#if 0
	for (i = 0; i < total_rows; i++)
	{
		for (j = 0; j <= cols; j++)
			printf ("%lf\t", array[i][j]);
		printf ("\n");
	}
#endif

	x = malloc (rows*sizeof(double));
	b = malloc (rows*sizeof(double));
	b_original = malloc (rows*sizeof(double));

	A = malloc (rows * (cols + 1) * sizeof (double));
	for (i = 0; i < rows; i++)
	{
		A[i] = malloc ((cols + 1) * sizeof (double));
	}

	if (rank == 0)
	{
		printf ("Number of CPUs used = %d\n", size);
		printf ("Average time taken is %lf\n", avgtime/size);
		j = 0;
		i = 0;
		while ((k = (j * size)) < rows)
		{
			A[k] = &(array[i][0]);
			i++;
			j++;
		}

		for (i = 1; i < size; i++)
		{
			j = 0;
			while ((k = (i + j * size)) < rows)
			{
				MPI_Recv(A[k], (cols + 1), MPI_DOUBLE, i, 99, MPI_COMM_WORLD, NULL);
				j++;
			}
		}

		back_substitution (rows, cols, A, x);


		FILE *fp;

		if ((fp = fopen (argv[1], "r")) == NULL)
		{
			printf ("File doesn't exists\n");
			return -1;
		}

		fseek(fp, 2*sizeof (int), SEEK_SET);	

		/* Get the A matrix */
		for (i = 0; i < rows; i++)
		{
			/* Fill the array */
			for (j = 0; j <= cols; j++)
			{
				fread(&(A[i][j]), sizeof (double), 1, fp);
				if (j == cols)
				{
					b_original[i] = A[i][j];
				}
			}
		}

		matrix_vector_multiplication (rows, cols, A, x, b);

		printf ("Final output difference: %lf\n", RMS_V_cmp(rows, b, b_original));

		fclose (fp);
		MPI_Finalize (); 

		return 0; 
	}

	else 
	{
		for (i = 0; i < total_rows; i++)
		{
			MPI_Send(&(array[i][0]), (cols + 1), MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize (); 

	return 0; 
} 
