#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int get_rows_cols (int *rows, int *cols, char *filename);

int guassian_elimination (int total_rows, int cols, int rank, int size, double **array);

int back_substitution (int rows, int cols, double **array, double *x);

void print_array(double *array, int cols);

void matrix_vector_multiplication (int rows, int cols, double **A, double *x, double *b);

double RMS_V_cmp(int n, double *x, double *y);
