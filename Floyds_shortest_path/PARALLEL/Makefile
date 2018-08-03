OBJ := main.o my_mpi.o parallel_floyd1.o 

floyd.exe : ${OBJ}
	mpicc -o $@ $^

${OBJ}:

clean:
	rm *.exe *.o


