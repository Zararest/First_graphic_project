#include "lib/headers/Matrix.h"

//g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system

int main(){

    FILE* input = fopen("matrix.txt", "r");

    Matrix obj_1(input);
    Matrix obj_2(3, 4, 2); 

    printf("obj_1:\n");
    obj_1.print(stdout);

    printf("obj_2:\n");
    obj_2.print(stdout);
}