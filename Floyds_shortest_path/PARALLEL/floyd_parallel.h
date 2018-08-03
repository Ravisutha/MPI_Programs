#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include "MyMPI.h"

void compute_shortest_path (int rows, int **array, MPI_Comm cart_comm, int *grid_size, MPI_Comm row_comm, MPI_Comm col_comm);
void compute_root (int k, int grid_size, int rows, int *root, int *index);
int compute_index (int k, int diff, int *tmp, int range);

int min (int a, int b);
int add (int a, int b);
int mod(int a);
