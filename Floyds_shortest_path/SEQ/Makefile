OBJ1 := make_graph.o graph.o
OBJ2 := print_graph.o graph.o
OBJ3 := floyd_serial.o graph.o seq_floyd.o

make_graph.exe : ${OBJ1}
	mpicc -o $@ $^

print_graph.exe : ${OBJ2}
	mpicc -o $@ $^

floyd_serial.exe : ${OBJ3}
	mpicc -o $@ $^

${OBJ1}:

${OBJ2}:

${OBJ3}:

clean:
	rm *.exe *.o
