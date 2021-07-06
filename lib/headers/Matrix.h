#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <math.h>

char my_getc(FILE* input);

class Matrix{

private:

    int rows_count;
    int columns_count;

    float* data;

    FILE* log;

public:

    Matrix(int vertical_size, int horizontal_size, float* numbers);//+

    Matrix(int vertical_size, int horizontal_size, float diag_elem);//+              //диагональная матрица с элементом diag_elem

    Matrix(FILE* input);//+

    Matrix(const Matrix& old_obj);   //+                                             //конструктор копирования

    Matrix(Matrix&& old_obj);//+                                                     //конструктор перемещения(для подачи в оператор присвыаивания итд)

    ~Matrix();//+

    void transpose();//+
    
    void print(FILE* output_stream);//+

    int length();//+

    float determinant();//+

    int is_quadratic();//+

    Matrix& operator = (const Matrix& rv);//+                                         //оператор присваивания для lvalue = lvalue

    Matrix& operator = (Matrix&& rv);//+                                              //оператор присваивания перемещением

    float* operator [] (const int row_number);//+                                           //после выполения этого оператора будет еще одно обращение [](нет проверки на выход за границы)(номера строк с 0)

    const float* operator [] (const int row_number) const;//+                         //оператор для константных объектов класса

    Matrix operator ^ (char degree);//+                                           //можно дописать обратную матрицу через знак -

    friend Matrix operator * (const Matrix& L_matr, const Matrix& R_matr);//+

    friend Matrix operator * (const Matrix& matr, int number);//+

    friend Matrix operator * (int number, const Matrix& matr);//+
 
    friend Matrix operator + (const Matrix& L_matr, const Matrix& R_matr);//+         //если не совпадают размеры, то ассерт

    friend Matrix operator - (const Matrix& L_matr, const Matrix& R_matr);//+    

    friend float cos(const Matrix& L_matr, const Matrix& R_matr);//+

    friend int same_size(const Matrix& L_matr, const Matrix& R_matr);//+   

    friend int could_be_mult(const Matrix& L_matr, const Matrix& R_matr);//+

    friend float minor(const Matrix& init_matr, int deleted_rows_count, int* deleted_columns);//+

    friend void print_minor(const Matrix& init_matr, int deleted_rows_count, int* deleted_columns);//+
};

#endif
