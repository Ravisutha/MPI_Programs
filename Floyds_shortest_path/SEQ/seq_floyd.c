/*Author : Ravisutha Sakreptna Srinivasamurthy
 * Program : Floyd's Alg implementation
 */
#include "floyd_seq.h"

void compute_shortest_path (int rows, int **array)
{
	int i, j, k;

	/* Start floyd alg */
	for (k = 0; k < rows; k++)
	{
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < rows; j++)
			{
				array[i][j] = min (array[i][j], add(array[i][k], array[k][j]));
			}
		}
	}
}

int min (int a, int b)
{
	if (a < 0)
	{
		return b;
	}
	else if (b < 0)
	{
		return a;
	}

	else if (a < b)
	{
		return a;
	}

	else 
	{
		return b;
	}
}

int add (int a, int b)
{
	if (a < 0)
		return -1;
	else if (b < 0)
		return -1;
	else
		return (a + b);
}
