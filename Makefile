main: main.o Matrix.o
	g++ -o main Matrix.o main.o

main.o: main.cpp 
	g++ -c main.cpp

Matrix.o: src/Matrix.cpp
	g++ -c src/Matrix.cpp

Object.o: src/Object.cpp src/Matrix.cpp src/Point.cpp
	g++ -c src/Object.cpp src/Matrix.cpp src/Point.cpp


test: src/Object.cpp src/Matrix.cpp src/Point.cpp test.cpp src/Sphere.cpp src/Flatness.cpp src/Space.cpp src/Elastic_interaction.cpp src/Physical_engine.cpp
	g++ src/Object.cpp src/Matrix.cpp src/Point.cpp test.cpp src/Sphere.cpp src/Flatness.cpp src/Space.cpp src/Elastic_interaction.cpp src/Physical_engine.cpp -o build/test



.PHONY : clean
clean:
	rm -rf *.o main
	rm -rf *.o build/test