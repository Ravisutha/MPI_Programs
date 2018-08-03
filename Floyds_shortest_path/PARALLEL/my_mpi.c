/* MyMPI.c -- A library of matrix/vector
 * input/output/redistribution functions.
 *
 * Programed by Michael J. Quinn
 * Last modification: 4 September 2002
 * "Parallel Programming in C with MPI and OpenMP"
 * Typed and modified by xiu_dim, 17 Nov. 2015
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyMPI.h"

int get_size(MPI_Datatype t)
{
	if (t == MPI_BYTE) return sizeof(char);
	if (t == MPI_DOUBLE) return sizeof(double);
	if (t == MPI_FLOAT) return sizeof(float);
	if (t == MPI_INT) return sizeof(int);
	printf("Error: Unrecognized argument to 'get_size'\n");
	fflush(stdout);
	MPI_Abort(MPI_COMM_WORLD, TYPE_ERROR);
}

void *my_malloc(
		int id,    /* IN - Process rank */
		int bytes) /* IN - Bytes to allocate */
{
	void *buffer;
	if ((buffer = malloc((size_t)bytes)) == NULL) {
		printf("Error:Malloc failed for process %d\n", id);
		fflush(stdout);
		MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
	}
	return buffer;
}
 
void read_checkerboard_matrix(
		char *s,            /* IN - File name */
		void ***subs,       /* OUT - 2-D array */
		void **storage,     /* OUT - Array elements */
		MPI_Datatype dtype, /* IN - Matrix element type */
		int *m,             /* OUT - Number of rows in matrix */
		int *n,             /* OUT - Number of cols in matrix */
		MPI_Comm grid_comm)      /* IN - Communicator */
{
	int p;              /* Number of processes */
	int grid_id;        /* Process rank in grid */
	int datum_size;     /* Size of matrix element */
	int coords[2];      /* Coords of process receiving next row of matrix */
	int grid_coords[2]; /* Coords of this process */
	int grid_period[2]; /* Wraparound */
	int grid_size[2];   /* Dimemsions of process grid */
	int local_rows;     /* Number of matrix rows in this process */
	int local_cols;     /* Number of matrix cols in this process */
	FILE *infileptr;    /* Input file poiter */
	void *buffer;       /* File buffer */
	void **lptr;        /* Pointer into 'subs' */
	void *rptr;         /* Pointer into 'storage' */
	void *laddr;        /* Used when process 0 gets row */
	void *raddr;        /* Address of first element to send */
	int dest_id;        /* Rank of receiving process */
	MPI_Status status;  /* Result of receive */
	int i,j,k;
	MPI_Comm_size(grid_comm, &p);
	MPI_Comm_rank(grid_comm, &grid_id);
	datum_size = get_size(dtype);
	/* Process 0 opens file, get numbers of rows and cols, 
	   and broadcasts this information to the other processes */
	if (grid_id == 0) {
		infileptr = fopen(s, "r");
		if (infileptr == NULL) *m = 0;
		else 
		{
			fread(m, sizeof(int), 1, infileptr);
			*n = *m;
		}
	}
	MPI_Bcast(m, 1, MPI_INT, 0, grid_comm);
	if (!(*m)) MPI_Abort(MPI_COMM_WORLD, OPEN_FILE_ERROR);
	MPI_Bcast(n, 1, MPI_INT, 0, grid_comm);
	/* Each process determines the size of the submatrix
	   it is responsible for. */
	MPI_Cart_get(grid_comm, 2, grid_size, 
			grid_period, grid_coords);
	local_rows = BLOCK_SIZE(grid_coords[0],grid_size[0],*m);
	local_cols = BLOCK_SIZE(grid_coords[1],grid_size[1],*n);
	/* Dynamicaly allocate two-dimensional matrix 'subs' */
	*storage = my_malloc(grid_id, 
			local_rows*local_cols*datum_size);
	*subs = (void **)my_malloc(grid_id, local_rows*PTR_SIZE);
	lptr = (void *)*subs;
	rptr = (void *)*storage;
	for (i=0; i<local_rows; ++i) {
		*(lptr++) = (void *)rptr;
		rptr += local_cols * datum_size;
	}
	/* Grid process 0 reads in the matrix one row at a time 
	   and distributes each row among the MPI processes */
	if (grid_id == 0)
		buffer = my_malloc(grid_id, (*n)*datum_size);
	/* For each row of processes in the process grid */
	for (i=0; i<grid_size[0]; ++i) 
	{
		coords[0] = i;
		/* For each matrix row controlled by this process row */
		for (j=0; j<BLOCK_SIZE(i,grid_size[0],*m); ++j) 
		{
			/* Read in a row of the matrix */
			if (grid_id == 0)
			{
				fread(buffer, datum_size, *n, infileptr);
			}
			/* Distribute it among processes in the grid row */
			for (k=0; k<grid_size[1]; ++k) {
				coords[1] = k;
				/* Find address of first element to send */
				raddr = buffer + 
					BLOCK_LOW(k,grid_size[1],*n) * datum_size;
				/* Determine the grid ID of the process
				   getting the subrow */
				MPI_Cart_rank(grid_comm, coords, &dest_id);
				if (grid_id == 0) {
					/* Process 0 is responsible for sending */
					if (dest_id == 0) {
						/* It is sending(copying) to itself */
						laddr = (*subs)[j];
						memcpy(laddr, raddr, 
								local_cols*datum_size);
					} else {
						/* It is sending to another process */
						MPI_Send(raddr, 
								BLOCK_SIZE(k,grid_size[1],*n), 
								dtype, dest_id, 0, grid_comm);
					}
				} else if (grid_id == dest_id) {
					/* Process 'dest_id' is responsible for receiving */
					MPI_Recv((*subs)[j], local_cols, dtype, 
							0, 0, grid_comm, &status);
				}
			}
		}
	}
	if (grid_id == 0) free(buffer);
	return;
}

void print_subvector(
		void *a,            /* IN - Singly subscripted array */
		MPI_Datatype dtype, /* IN - Type of elements */
		int n)              /* IN - Size of array */
{
	int i;
	for (i=0; i<n; ++i) {
		if (dtype == MPI_DOUBLE)
			printf("%6.3f ",((double *)a)[i]);
		else {
			if (dtype == MPI_FLOAT)
				printf("%6.3f ",((float *)a)[i]);
			else if (dtype == MPI_INT)
				printf("%6d ",((int *)a)[i]);
		}
	}
	return;
}

void print_checkerboard_matrix(
		void **a,           /* IN - 2D array */
		MPI_Datatype dtype, /* IN - Matrix element type */
		int m,              /* IN - Number of matrix rows */
		int n,              /* IN - Number of matrix columns */
		MPI_Comm grid_comm, char *filename) /* IN - Communicator */
{
	int p;              /* Number of processes */
	int grid_id;        /* Process rank in grid */
	int datum_size;     /* Size of matrix element */
	int coords[2];      /* Grid coords of process sending elements */
	int grid_coords[2]; /* Coords of this process */
	int grid_period[2]; /* Wraparound */
	int grid_size[2];   /* Dimemsions of process grid */
	int local_cols;     /* Number of matrix cols in this process */
	int els;            /* Element received */
	int src;            /* ID of process with subrow */
	void *buffer;       /* Buffer to hold one matrix row */
	void *laddr;        /* Address to put subrow */
	MPI_Status status;  /* Result of receive */
	int i,j,k;
	MPI_Comm_size(grid_comm, &p);
	MPI_Comm_rank(grid_comm, &grid_id);
	datum_size = get_size(dtype);
	MPI_Cart_get(grid_comm, 2, grid_size, 
			grid_period, grid_coords);

	FILE *fp = fopen (filename, "w");
	fwrite (&n, 4, 1, fp);

	if (fp == NULL)
	{
		printf ("Output file %s not found\n", filename);
		return;
	}
	local_cols = BLOCK_SIZE(grid_coords[1],grid_size[1],n);
	if (!grid_id) 
		buffer = my_malloc(grid_id, n*datum_size);
	/* For each row of the process grid */
	for (i=0; i<grid_size[0]; ++i) 
	{
		coords[0] = i;
		/* For each matrix row controlled by the process row*/
		for (j=0; j<BLOCK_SIZE(i,grid_size[0],m); ++j) 
		{			
			/* Collect the matrix row on grid process 0 and 
			   print it */
			if (!grid_id) 
			{
				for (k=0; k<grid_size[1]; ++k) 
				{
					coords[1] = k;
					MPI_Cart_rank(grid_comm, coords, &src);
					els = BLOCK_SIZE(k,grid_size[1],n);
					laddr = buffer + 
						BLOCK_LOW(k,grid_size[1],n) * datum_size;
					if (src == 0) 
					{
						memcpy(laddr, a[j], els*datum_size);
					} 
					else 
					{
						MPI_Recv(laddr, els, dtype, src, 0, 
								grid_comm, &status);
					}
				}

				/* If number of rows is greater than 10 then don't write it into the console */
				if (n < 10)
				{
					print_subvector(buffer, dtype, n);
					putchar('\n');
				}

				/* Write the output to a file */
				fwrite (buffer, 4, n, fp);

			} else if (grid_coords[0] == i) {
				MPI_Send(a[j], local_cols, dtype, 0, 0, grid_comm);
			}
		}
	}
	if (!grid_id) {
		free(buffer);
		putchar('\n');
	}
	fclose (fp);
	return;
}
