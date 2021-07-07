main: main.o Matrix.o
	g++ -o main Matrix.o main.o

main.o: main.cpp 
	g++ -c main.cpp

Matrix.o: lib/Matrix.cpp
	g++ -c lib/Matrix.cpp

Object.o: lib/Object.cpp lib/Matrix.cpp lib/Point.cpp
	g++ -c lib/Object.cpp lib/Matrix.cpp lib/Point.cpp


test: lib/Object.cpp lib/Matrix.cpp lib/Point.cpp test.cpp lib/Sphere.cpp lib/Flatness.cpp lib/Space.cpp lib/Elastic_interaction.cpp
	g++ lib/Object.cpp lib/Matrix.cpp lib/Point.cpp test.cpp lib/Sphere.cpp lib/Flatness.cpp lib/Space.cpp lib/Elastic_interaction.cpp -o test



.PHONY : clean
clean:
	rm -rf *.o main
	rm -rf *.o test