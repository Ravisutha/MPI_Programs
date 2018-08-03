/* 
 * Author : Ravisutha Sakrepatna Srinivasamurthy 
 * Assignment: Floyd's Algorithm
 * Class: Intro to Parallel Systems */

#include "floyd_parallel.h"

void compute_shortest_path (int rows, int **array, MPI_Comm cart_comm, int *grid_size, MPI_Comm row_comm, MPI_Comm col_comm)
{
	/* Declarations goes here */
	int i, j, k, root, index, g; 
	int coords1[2], coords2[2];
	int id;

	MPI_Comm_rank (cart_comm, &id);
	
	/* Row_size: Number of rows 
	 * Col_size: Number of cols */
	int row_size = BLOCK_SIZE(id, grid_size[0], rows);
	int col_size = BLOCK_SIZE(id, grid_size[1], rows);

	/* Buffer_row: Used for transferring or receving a row
	 * Buffer_col: Used for transferring or receving a column */
	int buffer_row[row_size], buffer_col[col_size];

	/* coords2: Co-ordinates of this process
	 * coords1: Co-ordinates of the root process */
	MPI_Cart_coords(cart_comm, id, 2, coords2);

	/* -----------------------------------------------Floyds ALG begins------------------------------------------ */
	for (k = 0; k < rows; k++)
	{
		/**************************Column Broad Cast STARTS *************************/
		compute_root (k, grid_size[0], rows, &root, &index);         //Compute the root and index of the kth owner

		if (root == coords2[0])                                      //If this process itself is the root then broadcast the kth row
		{
			for (g = 0; g < col_size; g++)
			{
				buffer_row[g] = array[index][g];
			}
		}

		MPI_Bcast (buffer_row, col_size, MPI_INT, root, col_comm);	
		/**************************Column Broad Cast ENDS *************************/

		/**************************ROW Broad Cast STARTS *************************/
		compute_root (k, grid_size[1], rows, &root, &index);         //Compute the root and index of the kth owner

		if (root == coords2[1])                                      //If this process itself is the root then broadcast the kth column

		{
			for (g = 0; g < row_size; g++)
			{
				buffer_col[g] = array[g][index];
			}
		}

		MPI_Bcast (buffer_col, row_size, MPI_INT, root, row_comm);	
		/**************************ROW Broad Cast ENDS *************************/
		
		/* Compute Shortest Path */
		for (i = 0; i < row_size; i++)
		{
			for (j = 0; j < col_size; j++)
			{
				array[i][j] = min (array[i][j], add(buffer_col[i], buffer_row[j]));

			}
		}
	}
	/* -----------------------------------------------Floyds ALG ends---------------------------------------------- */
}

/*To calculate the min of two numbers */
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

/* Add two numbers(-1 is considered as very large number)*/
int add (int a, int b)
{
	if (a < 0)
		return -1;
	else if (b < 0)
		return -1;
	else
		return (a + b);
}

void compute_root (int k, int grid_size, int rows, int *root, int *index)
{
	int offset;

	*root = BLOCK_OWNER (k, grid_size, rows);

	offset = BLOCK_LOW(*root, grid_size, rows);

	*index = k - offset;

}
