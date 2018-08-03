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

	return 0;
}

int guassian_elimination (int rows, int cols, char *filename, double ***array)
{
	int i,j, k;
	FILE *fp;

	if ((fp = fopen (filename, "r")) == NULL)
	{
		printf ("File doesn't exists\n");
		return -1;
	}

	*array = malloc (rows * sizeof (double *));

	fseek(fp, 2*sizeof (int), SEEK_SET);	

	/* Get the A matrix */
	for (i = 0; i < rows; i++)
	{
		(*array)[i] = malloc ((cols + 1)*sizeof(double));

		/* Fill the array */
		for (j = 0; j <= cols; j++)
		{
			fread(&((*array)[i][j]), sizeof (double), 1, fp);
		}
	}

	//printf ("rows = %d\n", rows);
	//printf ("cols = %d\n", cols);

	/* Do Guassian Elimination */
	for (i = 1; i < rows; i++)
	{
		for (k = i; k < rows; k++)
		{
			for (j = 0; j <= cols; j++)
			{
				(*array)[k][j] = (*array)[k][j] - (*array)[i - 1][j] * (*array)[k][i - 1]/ (*array)[i - 1][i - 1];
			}
		}
	}
#if 0
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf ("%lf  ", (*array)[i][j]);
		}
		printf ("\n");
	}
#endif
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

		//printf ("x[%d] = %lf\n", i, x[i]);
	}
}

void matrix_vector_multiplication (int rows, int cols, double **A, double *x, double *b)
{
	int i, j;

	//printf ("Output vector:\n");
	for (i = 0; i < rows; i++)
	{
		b[i] = 0;
		for (j = 0; j < rows; j++)
		{
			b[i] += A[i][j] * x[j];
		}

		//printf ("%lf\n", b[i]);
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

