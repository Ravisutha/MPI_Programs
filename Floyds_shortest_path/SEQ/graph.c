/* Author: Ravisutha Sakrepatna Srinivasmurthy 
 * Program: All programs related to sequential operation
 * */
#include "graph.h"

int make_graph_file (char *file_name, int rows, int range1, int range2)
{
	/* Declaration goes here */
	int i = 0, j = 0, number = 0;
	time_t *t = NULL;
	FILE *fptr = fopen (file_name, "w+");

	if (fptr == NULL)
	{
		printf ("Could not access the file\n");
		return -1;
	}
	
	//Write the values of number of rows and columns
	fwrite (&rows, 4, 1, fptr);

	srand(time(t));
	
	/* Get random numbers and fill the file with matrix */
	for (i = 0; i < rows; i++)
		for (j = 0; j < rows; j++)
		{
			number = rand() % range1;

			if (i == j)
			{
				number = 0;
			}

			else if (number > range2)
			{
				number = -1;
			}

			
			fwrite (&number, 4, 1, fptr);
		}

	fclose(fptr);
	return 0;
}

int read_file (char *filename, int *rows, int ***array)
{
	int i, j;

	FILE *fptr = fopen (filename, "r");

	if (fptr == NULL)
	{
		printf ("Could not access the file\n");
		return -1;
	}
	
	//Read the values of number of rows and columns
	fread (rows, 4, 1, fptr);

	//Create "rows" number of pointers
	if ((*array = malloc (*rows * sizeof (int *))) == NULL)
	{
			printf ("Malloc error\n");
			fflush (stdout);
	}

	fflush (stdout);
	for (i = 0; i < *rows; i++)
	{
		(*array)[i] = malloc (*rows * sizeof (int));
	}

	//Finally read the file
	for (i = 0; i < *rows; i++)
		for (j = 0; j < *rows; j++)
		{
			fread ( &(*array)[i][j], 4, 1, fptr);
		}

	fclose(fptr);

	return 0;
}

void print_file (int **array, int rows)
{
	int i, j;

	printf (" \t |\t");
	for (i = 0; i < rows; i++)
	{
		printf ("%d\t", i);
	}

	printf ("\n-----------------------------------------------------\n");

	//Print the content of the file
	for (i = 0; i < rows; i++)
	{
		printf ("%d\t |\t", i);

		for (j = 0; j < rows; j++)
		{
			printf ("%d\t", array[i][j]);	
		}

		printf ("\n");
	}
}


int write_graph_file (char *file_name, int rows, int **array)
{
	int i = 0, j = 0, number = 0;

	FILE *fptr = fopen (file_name, "w");

	if (fptr == NULL)
	{
		printf ("Could not access the file\n");
		return -1;
	}
	
	//Write the values of number of rows and columns
	fwrite (&rows, 4, 1, fptr);

	/* Write the content of the array to a file*/
	for (i = 0; i < rows; i++)
		for (j = 0; j < rows; j++)
			fwrite ( &array[i][j], 4, 1, fptr);

	fclose(fptr);

	return 0;
}

int change_file_name (char *filename)
{
	int i, j;

	char *new_format = "seq";

	if (filename == NULL)
	{
		return BAD_FILENAME;
	}

	while (filename[i] != '\0' && filename[i] != '.')
	{
		i++;
	}

	if (filename[i] == '\0')
	{
		return BAD_FORMAT;
	}
	
	/* Change the format name */
	if (filename[i] == '.')
	{
		i++;
		for (j = 0; j < 3; j++)
		{
			filename[i+j] = new_format[j];
		}

		filename[i+j] = '\0';
	}
}
