#include "headers/Matrix.h"

Matrix::Matrix(int horizontal_size, int vertical_size, float* numbers){//копирует данные из массива

    printf("Конструктор матрицы по массиву\n\n");

    if (numbers != NULL){

        data = (float*)calloc(horizontal_size * vertical_size, sizeof(float));
        row_count = vertical_size;
        columns_count = horizontal_size;

        memcpy(data, numbers, horizontal_size * vertical_size * sizeof(float));
    } else{

        data = (float*)calloc(horizontal_size * vertical_size, sizeof(float));
        row_count = vertical_size;
        columns_count = horizontal_size;

        for (int i = 0; i < horizontal_size * vertical_size; i++){

            data[i] = 0;
        }
    }
}

Matrix::Matrix(int horizontal_size, int vertical_size, float diag_elem){//заполняет таблицу нулями и если матрица квадратная, то диагональ это diag_elem

    printf("Конструктор диагональной матрицы\n\n");

    if (horizontal_size * vertical_size != 0){

        data = (float*)calloc(horizontal_size * vertical_size, sizeof(float));
    } else{

        data = NULL;
    }
    
    row_count = vertical_size;
    columns_count = horizontal_size;

    for (int i = 0; i < horizontal_size * vertical_size; i++){

        data[i] = 0;
    }

    if (row_count == columns_count){

        for (int i = 0; i < row_count; i++){

            data[i * row_count + i] = diag_elem;
        }
    }
}

char my_getc(FILE* input){

    char c = '\0';
    int count = 0;

    while (((c = getc(input)) == ' ') || (c == '\t')){
        
        count++;
    }

    if (count != 0){

        fseek(input, -1, SEEK_CUR);
    } 
    
    if (c == EOF){

        c = '\n';
    }
    
    return c;
}

Matrix::Matrix(FILE* input){

    printf("Конструктор по файлу\n\n");

    int cur_size = 0, tmp_columns_count = 0;
    char next_symbol = '\0';
    float tmp = 0;

    data = NULL;
    row_count = 0;
    columns_count = 0;

    if (input != NULL){

        int scanf_ret = fscanf(input, "%f", &tmp);
        
        while (scanf_ret != -1){

            cur_size++;

            data = (float*)realloc(data, cur_size * sizeof(float));
            data[cur_size - 1] = tmp;

            if (my_getc(input) == '\n'){
                
                if (columns_count != 0){

                    assert(cur_size - row_count * columns_count == columns_count);
                } else{

                    columns_count = cur_size;
                }

                row_count++;
            }

            scanf_ret = fscanf(input, "%f", &tmp);
        }

        fseek(input, 0, SEEK_SET); 
    }

    

    printf("in constructor row_count = %i and col_count = %i\n", row_count, columns_count);
}

Matrix::Matrix(const Matrix& old_obj){

    printf("Конструктор глубокого копирования\n\n");

    row_count = old_obj.row_count;
    columns_count = old_obj.columns_count;

    memcpy(data, old_obj.data, row_count * columns_count * sizeof(float));
}

Matrix::Matrix(Matrix&& old_obj){

    printf("Конструктор перемещения\n\n");

    data = old_obj.data;
    row_count = old_obj.row_count;
    columns_count = old_obj.columns_count;

    old_obj.data = NULL;
    old_obj.row_count = 0;
    old_obj.columns_count = 0;
}

Matrix::~Matrix(){

    printf("Деструктор\n\n");

    if (data != NULL){

        data[0] = -228; //проверка что старые данные не используются
    }
    
    free(data);
}

void Matrix::transpose(){

    float tmp = 0;
    float* new_data = (float*)calloc(row_count * columns_count, sizeof(float));

    for (int i = 0; i < row_count; i++){

        for (int j = 0; j < columns_count; j++){

            assert(i * columns_count + j < row_count * columns_count);

            new_data[j * row_count + i] = data[i * columns_count + j];
        }
    }

    tmp = columns_count;
    columns_count = row_count;
    row_count = (int)tmp;

    free(data);
    data = new_data;
}

void Matrix::print(FILE* output_stream){

    for (int i = 0; i < row_count * columns_count; i++){

        fprintf(output_stream, "%f ", data[i]);

        if ((i + 1) % columns_count == 0){

            fprintf(output_stream, "\n");
        }
    }
}

int max(int left, int right){

    if (left > right){

        return left;
    } else{

        return right;
    }
}

int Matrix::length(){

    if (columns_count == 1){

        return max(row_count, columns_count);
    } else{

        return -1;
    }
}

Matrix& Matrix::operator = (const Matrix& rv){

    printf("Оператор присваивания lvalue = lvalue\n\n");

    if (this == &rv){

        return *this;
    }
        
    data = (float*)calloc(rv.row_count * rv.columns_count, sizeof(float));
    
    row_count = rv.row_count;
    columns_count = rv.columns_count;

    memcpy(data, rv.data, row_count * columns_count * sizeof(float));

    return *this;
}

Matrix& Matrix::operator = (Matrix&& rv){

    printf("Оператор присваивания перемещения lvalue = rvalue\n\n");

    data = rv.data;
    row_count = rv.row_count;
    columns_count = rv.columns_count;

    rv.data = NULL;
    rv.row_count = 0;
    rv.columns_count = 0;

    return *this;
}

float* Matrix::operator [] (int row_number){

    assert(row_number < row_count);
    return &data[row_number * columns_count];
}

Matrix operator + (const Matrix& L_matr, const Matrix& R_matr){

    printf("Оператор +\n\n");

    assert((L_matr.row_count == R_matr.row_count) && (L_matr.columns_count == R_matr.columns_count));

    float* new_data = (float*)calloc(L_matr.row_count * L_matr.columns_count, sizeof(float));

    for (int i = 0; i < L_matr.row_count * L_matr.columns_count; i++){

        new_data[i] = L_matr.data[i] + R_matr.data[i];
    }

    Matrix tmp_obj(L_matr.columns_count, L_matr.row_count, new_data);
        
    free(new_data);

    return static_cast<Matrix&&>(tmp_obj); 
}