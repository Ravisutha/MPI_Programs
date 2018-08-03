/* * * * * * * * * * * * * * *
 * Program: Gaussian Sequenctial Execution
 * Author : Ravisutha Sakrepatna
 * * * * * * * * * * * * * * */

#include "header.h"

int main (int argc, char *argv[])
{
	/* Declarations goes here */
	int rows, cols;
	double **array, *x;

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

	/* Do gaussian elimination */
	guassian_elimination (rows, cols, argv[1], &array);

	x = malloc (rows * sizeof (double));

	/* Back substitution to get back the result */
	back_substitution (rows, cols, array, x);

	return 0;
}
