#include "header.h"

int main (int argc, char *argv[])
{
	/* Declarations goes here */
	int rows, cols, i, j;
	double **array, *x, *b, *b_original;
	time_t starttime, endtime;

	/* Check for absence of file name */
	if (argc < 2)
	{
		printf ("Input the file name\n");
		return -1;
	}

	/* Get row and column information */
	if (get_rows_cols (&rows, &cols, argv[1]) == -1)
	{
		printf ("File doesn't exists\n");
		return -1;
	}

	/* Check if the rows n cols are the same */
	if (rows != cols)
	{
		if (rows > cols)
			printf ("Not possible to compute solutions, overdetermined system\n");
		if (rows < cols)
			printf ("Not possible to compute solutions, underdetermined system\n");
		return -1;
	}
	starttime = time (NULL);
	/* Do gaussian elimination */
	guassian_elimination (rows, cols, argv[1], &array);
	endtime = time(NULL);

	printf ("Time taken for %d matrix is %d\n", rows, endtime - starttime);
	x = malloc (rows * sizeof (double));
	b = malloc (rows * sizeof (double));
	b_original = malloc (rows * sizeof (double));

	/* Back substitution to get back the result */
	back_substitution (rows, cols, array, x);

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
			fread(&(array[i][j]), sizeof (double), 1, fp);
			//printf ("%lf \t",array[i][j]);

			if (j == cols)
			{
				b_original[i] = array[i][j];
				printf ("\nb[i] = %lf\n", array[i][j]);
			}
		}
	}
	matrix_vector_multiplication (rows, cols, array, x, b);	

	printf ("Difference is %lf\n", RMS_V_cmp(rows, b, b_original));

	return 0;
}
