#include "lib/headers/Matrix.h"

//g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system

#include <ctime>

int main(){ //const Matrix& это rvalue, поэтому с ним возникают проблемы

    FILE* input = fopen("matrix.txt", "r");
    FILE* input_2 = fopen("matrix_2.txt", "r");
    FILE* input_3 = fopen("matrix_3.txt", "r");
    float* arr = (float*)calloc(8, sizeof(float));

    for (int i = 0; i < 8; i++){
        
        fscanf(input_3, "%f", &arr[i]);
    }
    
    Matrix obj_1(input);
    Matrix* arr_of_matr = (Matrix*)calloc(10000006, sizeof(Matrix));
    
    assert(arr_of_matr != NULL);

    printf("sizof(Matrix) = %li\n", sizeof(Matrix));

    long unsigned int start_time =  clock(); 
    for (int i = 0; i < 10000000; i++){

        printf("i = %i\n", i);
        if (i % 2 == 0){

            arr_of_matr[i] =  (obj_1^'T') * obj_1;
        } else{

            arr_of_matr[9999999 - i] =  (obj_1^'T') * obj_1;
        }
    }
    long unsigned int end_time =  clock(); 

    printf("time = %li\n", end_time - start_time);
    fclose(input);
    fclose(input_2);
    fclose(input_3);
}