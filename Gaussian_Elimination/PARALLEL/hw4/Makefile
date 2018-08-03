OBJ1 := main.o guassian.o

make_graph.exe : ${OBJ1}
	mpicc -o $@ $^

${OBJ1}:

clean:
	rm *.exe *.o
