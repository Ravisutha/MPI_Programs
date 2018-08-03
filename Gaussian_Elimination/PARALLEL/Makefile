OBJ1 := main.o guassian.o

guassian.exe : ${OBJ1}
	mpicc -o $@ $^

${OBJ1}:

clean:
	rm *.exe *.o
