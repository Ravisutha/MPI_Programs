/* MyMPI.h
 *
 * Header file for a library of matrix/vector
 * input/output/redistribution functions.
 *
 * Programed by Michael J. Quinn
 * Last modification: 4 September 2002
 * "Parallel Programming in C with MPI and OpenMP"
 * Typed and modified by xiu_dim, 17 Nov. 2015
 */
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************** MACROS *********************/
#define DATA_MSG        0
#define PROMPT_MSG      1
#define RESPONSE_MSG    2

#define OPEN_FILE_ERROR -1
#define MALLOC_ERROR    -2
#define TYPE_ERROR      -3

#define PTR_SIZE        (sizeof(void *))
#define MIN(a,b)        ((a)<(b)?(a):(b))
#define CEILING(i,j)    (((i)+(j)-1)/(j))

#define BLOCK_LOW(id,p,n)   ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n)  \
            (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(j,p,n)  ((p*(j+1)-1)/(n))


/************ MISCELLANEOUS FUNCTIONS ************/
int get_size(MPI_Datatype);
void* my_malloc(int, int);
void terminate(int, char*);

/********** DATA DISTRIBUTION FUNCTIONS **********/
void create_mixed_xfer_arrays(int, int, int, 
        int**, int**);
void create_uniform_xfer_arrays(int, int, int, 
        int**, int**);
void replicate_block_vector(void*, int, void*, 
        MPI_Datatype, MPI_Comm);

/**************** INPUT FUNCTIONS ****************/
void read_row_striped_matrix(char*, void***, void**, 
        MPI_Datatype, int*, int*, MPI_Comm);
void read_col_striped_matrix(char*, void***, void**, 
        MPI_Datatype, int*, int*, MPI_Comm);
void read_checkerboard_matrix(char*, void***, void**, 
        MPI_Datatype, int*, int*, MPI_Comm);
void read_block_vector(char*, void**, MPI_Datatype, 
        int*, MPI_Comm);
void read_replicated_vector(char*, void**, MPI_Datatype, 
        int*, MPI_Comm);

/**************** OUTPUT FUNCTIONS ***************/
void print_submatrix(void**, MPI_Datatype, int, int);
void print_subvector(void*, MPI_Datatype, int);
void print_row_striped_matrix(void**, MPI_Datatype, 
        int, int, MPI_Comm);
void print_col_striped_matrix(void**, MPI_Datatype, 
        int, int, MPI_Comm);
void print_checkerboard_matrix(void**, MPI_Datatype,    
        int, int, MPI_Comm, char *filename);
void print_block_vector(void*, MPI_Datatype, int, MPI_Comm);
void print_replicated_vector(void*, MPI_Datatype, int, MPI_Comm);
