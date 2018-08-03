OBJ1 := drive.o guassian.o

guassian.exe : ${OBJ1}
	mpicc -o $@ $^

print_graph.exe : ${OBJ2}
	mpicc -o $@ $^

gaussian_serial.exe : ${OBJ3}
	mpicc -o $@ $^

${OBJ1}:

${OBJ2}:

${OBJ3}:

clean:
	rm *.exe *.o
