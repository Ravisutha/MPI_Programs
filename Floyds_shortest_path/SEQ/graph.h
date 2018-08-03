#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BAD_FORMAT -2
#define BAD_FILENAME -1

int make_graph_file (char *file_name, int rows, int range1, int range2);

int read_file (char *filename, int *rows, int ***array);

void print_file (int **array, int rows);

int write_graph_file (char *file_name, int rows, int **array);

int change_file_name (char *filename);
