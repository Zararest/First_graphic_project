#include "lib/headers/Matrix.h"

//g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system

void function(Matrix& func_obj){

    printf("вызов функции\n\n");

    printf("obj_3[0][0] = %f\n\n", func_obj[0][2]);

    //func_obj[0][0] = 1.5;
}

int main(){ //const Matrix& это rvalue, поэтому с ним возникают проблемы

    FILE* input = fopen("matrix.txt", "r");
    FILE* input_2 = fopen("matrix_2.txt", "r");

    Matrix obj_1(input);
    Matrix obj_2(input_2);
    Matrix obj_3(1, 1, (float)0);

    obj_3 = obj_1 * obj_2;

    obj_3.print(stdout);

    fclose(input);
    fclose(input_2);
}