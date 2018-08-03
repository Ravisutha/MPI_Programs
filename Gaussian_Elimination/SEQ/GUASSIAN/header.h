#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int get_rows_cols (int *rows, int *cols, char *filename);

int guassian_elimination (int rows, int cols, char *filename, double ***array);

int back_substitution (int rows, int cols, double **array, double *x);

void matrix_vector_multiplication (int rows, int cols, double **A, double *x, double *b);

double RMS_V_cmp(int n, double *x, double *y);
