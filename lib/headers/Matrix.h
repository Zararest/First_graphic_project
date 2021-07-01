#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>

class Matrix{

private:

    int row_count;
    int columns_count;

    float* data;

    FILE* log;

public:

    Matrix(int horizontal_size, int vertical_size, float* numbers);//+

    Matrix(int horizonlal_size, int vertical_size, float diag_elem);//+              //диагональная матрица с элементом diag_elem

    Matrix(FILE* input);//+

    Matrix(const Matrix& old_obj);   //+                                             //конструктор копирования

    Matrix(Matrix&& old_obj);//+                                                     //конструктор перемещения(для подачи в оператор присвыаивания итд)

    ~Matrix();//+

    void transpose();//+
    
    void print(FILE* output_stream);//+

    int length();//+

    float determinant();

    Matrix& operator = (const Matrix& rv);//+                                         //оператор присваивания для lvalue = lvalue

    Matrix& operator = (Matrix&& rv);//+                                              //оператор присваивания перемещением

    float* operator [] (int row_number);//+                                           //после выполения этого оператора будет еще одно обращение [](нет проверки на выход за границы)(номера строк с 0)

    friend Matrix operator * (const Matrix& L_matr, const Matrix& R_matr);

    friend Matrix operator * (const Matrix& L_matr, int number);

    friend Matrix operator * (int number, const Matrix& R_matr);
 
    friend Matrix operator + (const Matrix& L_matr, const Matrix& R_matr);//+         //если не совпадают размеры, то ассерт

    friend Matrix operator - (const Matrix& L_matr, const Matrix& R_matr);    

    friend int same_size(const Matrix& L_matr, const Matrix& R_matr);    
};

