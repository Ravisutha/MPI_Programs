#include "header.h"

int get_rows_cols (int *rows, int *cols, char *filename)
{
	FILE *fp = fopen (filename, "r");

	if (fp == NULL)
	{
		printf ("File doesn't exists");
		return -1;
	}

	/* Read number of rows and cols */
	fread(rows, sizeof (int), 1, fp);
	fread(cols, sizeof (int), 1, fp);

	fclose (fp);
	return 0;
}

int guassian_elimination (int total_rows, int cols, int rank, int size, double **array)
{
	int i,j, k, row_number, next_process, pre_process, pre_row_number, m, initial_value; 
	double temp[cols + 1], zero_out;
	MPI_Status status;

	if (rank == size - 1)
	{
		next_process = 0;
		pre_process = rank - 1;
	}

	else if (rank == 0)
	{
		next_process = rank + 1;
		pre_process = size - 1;
	}
	else
	{
		next_process = rank + 1;
		pre_process = rank - 1;
	}

	if (rank == 0)
	{
		initial_value = 1;
		MPI_Send (&(array[0][0]), (cols + 1), MPI_DOUBLE, next_process, 0, MPI_COMM_WORLD);	
	}
	else
	{
		initial_value = 0;
	}

	pre_row_number = 0;

	/* Do Guassian Elimination */
	for (i = initial_value; i < total_rows; i++)
	{
		row_number = i * size + rank;

		for (j = pre_row_number; j < row_number; j++)
		{
			/* If the jth row doesn't belongs the same process */
			if (((rank - j) % size) != 0)
			{
				MPI_Recv (temp, (cols + 1), MPI_DOUBLE, pre_process, j, MPI_COMM_WORLD, &status);	

				/* If the next process is the origin */
				if (((next_process - j) % size) != 0)
				{
					MPI_Send (temp, (cols + 1), MPI_DOUBLE, next_process, j, MPI_COMM_WORLD);	
				}
				for (m = i; m < total_rows; m++)
				{
					zero_out = array[m][j];
					for (k = 0; k <= cols; k++)
					{
						array[m][k] = array[m][k] - temp[k] * zero_out/temp[j];
					}
				}

			}

			else
			{
				for (m = i; m < total_rows; m++)
				{
					zero_out = array[m][j];
					for (k = 0; k <= cols; k++)
					{
						array[m][k] = array[m][k] - array[(j - rank) / size][k] * zero_out/ array[(j - rank) / size][j];
					}
				}

			}

		}

		MPI_Send (&(array[i][0]), (cols + 1), MPI_DOUBLE, next_process, row_number, MPI_COMM_WORLD);	
		pre_row_number = row_number;
	}
#if 0
	/* Print the output matrix */
	for (i = 0; i < total_rows; i++)
	{
		for (j = 0; j <= cols; j++)
		{
			printf ("%lf  \t", array[i][j]);
		}
		printf ("\n");
		printf ("\n");
	}
#endif
}

void print_array(double *array, int cols)
{
	int i, j;

	for (j = 0; j <= cols; j++)
	{
		printf ("%lf  \t", array[j]);
	}
	printf ("\n");
}

int back_substitution (int rows, int cols, double **array, double *x)
{
	/* Declaration goes here */
	int i, j;
	double temp;

	/* Calculate xn */
	x[rows - 1] = array[rows - 1][cols] / array[rows - 1][cols - 1];

	for (i = (rows- 2); i >= 0; i--)
	{
		temp = 0;

		for (j = (rows - 1); j > i; j--)
		{
			temp += x[j] * array[i][j];
		}

		x[i] = (array[i][cols] - temp) / array[i][i];
	}
}

void matrix_vector_multiplication (int rows, int cols, double **A, double *x, double *b)
{
	int i, j;

	for (i = 0; i < rows; i++)
	{
		b[i] = 0;
		for (j = 0; j < rows; j++)
		{
			b[i] += A[i][j] * x[j];
		}
		//printf ("b[%d] = %lf\n", i, b[i]);
	}
}

double RMS_V_cmp(int n, double *x, double *y)
{

	double sum = 0.0;

	int i;

	for (i = 0; i < n; i++)
	{

		double d;

		d = x[i] - y[i];

		sum += d * d;

	}

	return abs(sqrt(sum));

}
