main: main.o Matrix.o
	g++ -o main Matrix.o main.o

main.o: main.cpp 
	g++ -c main.cpp

Matrix.o: lib/Matrix.cpp
	g++ -c lib/Matrix.cpp

.PHONY : clean
clean:
	rm -rf *.o main