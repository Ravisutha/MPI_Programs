/* Author : Ravisutha Sakrepatna Srinivasamurthy
 * Program: Driver for make graph
 */
#include "graph.h"
#include "floyd_seq.h"

int main (int argc, char *argv[])
{
	int option = 0, rows = 0, range1 = 0, range2 = 0, len = 0;

	int **array;

	char *filename;

	//Parse the options
	while ((option = getopt(argc, argv,"n:r:p:o:")) != -1) 
	{
		switch (option) {
			case 'n' : rows = atoi(optarg); /* Number of rows */
				   break;

			case 'r' : range2 = atoi(optarg); /* Range2 */
				   break;

			case 'p' : range1 = atoi(optarg); /* Range1 */
				   break;

			case 'o' : len = strlen (optarg);
				   filename = malloc (len + 1);
				   strcpy (filename, optarg); /* File name */
				   break;

			default: printf("Unknown argument\n"); 

				 exit(EXIT_FAILURE);
		}
	}

	//Call make graph
	if (make_graph_file (filename, rows, range1, range2) == 0)
	{
		printf ("Written to the file %s\n", filename);
	}

	else 
	{
		printf ("Write unsuccessful\n");
	}
}
