/* Author : Ravisutha Sakrepatna Srinivasamurthy
 * Program : Print Graph driver
 */
#include "graph.h"
#include "floyd_seq.h"

int main (int argc, char *argv[])
{
	int option = 0, rows = 0, range1 = 0, range2 = 0, len = 0;

	int **array;

	char *filename;

	if (argc < 2)
	{
		printf ("Enter the file name\n");
		return -1;
	}
	
	//Get the filename
	filename = argv[1];

	//Read the file
	if (read_file (filename, &rows, &array) == -1)
	{
		printf ("ERROR: Reading a file\n");
	}
	
	//Print the file
	print_file (array, rows);	
}
