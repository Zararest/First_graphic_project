#include "headers/Objects.h"

char fgetc_without_space(FILE* input_file){

    char c = fgetc(input_file);

    while ((c == ' ') || (c == '\t')){

        c = fgetc(input_file);
    }

    return c;
}

void go_to_end_of_line(FILE* file){

    int counter = 0;
    char c = '\0';

    fseek(file, -1, SEEK_CUR);

    while ( ((c = fgetc(file)) != '|') && (c != EOF)){
        
        counter++;
    }
    fseek(file, -1, SEEK_CUR);

    if (counter != 0){
        
        if (c == '|'){

            printf("Due to error file pointer has been displaced by %i bytes\n", counter);
        } else{

            printf("Due to error file pointer has been displaced by %i bytes(now in the end of file)\n", counter);
        }   
    }
}

Object::Object(){

    color = (int*)calloc(3, sizeof(int));

    transpendecy = 0;   //прозрачность
    surface = 0;        //параметр рассеивания при отражении
    color[0] = 228;
    color[1] = 228;
    color[2] = 228;
    number_of_param = 0; 

    phys_param = NULL;
}

Object::~Object(){

    transpendecy = 0; 
    surface = 0;        
    number_of_param = 0; 

    free(color);
    free(phys_param);
}

Matrix get_vector(FILE* input_file){

    float tmp_vector[3] = {0, 0, 0};

    if (fgetc_without_space(input_file) != '{'){

        printf("Incorrect vector data in file[%ld](can't find '{')\n",  ftell(input_file));
        go_to_end_of_line(input_file);
    }

    for (int i = 0; i < 3; i++){

        if (fscanf(input_file, "%f", &tmp_vector[i]) <= 0){

            printf("Incorrect vector data in file[%ld](too few coordinates)\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
    }

    if (fgetc_without_space(input_file) != '}'){

        printf("Incorrect vector data in file[%ld](can't find '}')\\n",  ftell(input_file));
        go_to_end_of_line(input_file);
    }

    Matrix tmp_matr(3, 1, tmp_vector);

    return static_cast<Matrix&&>(tmp_matr);
}

Point get_point(FILE* input_file){

    float tmp[3] = {0, 0, 0};
    Point tmp_obj;

    if (fgetc_without_space(input_file) != '('){

        printf("Incorrect point data in file[%ld](can't find '(')\n",  ftell(input_file));
        go_to_end_of_line(input_file);
    }

    for (int i = 0; i < 3; i++){

        if (fscanf(input_file, "%f", &tmp[i]) <= 0){

            printf("Incorrect point data in file[%ld](to few coordinates)\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
    }

    if (fgetc_without_space(input_file) != ')'){

        printf("Incorrect point data in file[%ld](can't find ')')\n",  ftell(input_file));
        go_to_end_of_line(input_file);
    }

    tmp_obj.x = tmp[0];
    tmp_obj.y = tmp[1];
    tmp_obj.z = tmp[2];

    return static_cast<Point&&>(tmp_obj);
}

void get_object_params(FILE* input_file, Object& tmp_obj){

    float tmp = 0; 

    if (fgetc_without_space(input_file) != '{'){

        printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    }  

    for (int i = 0; i < 3; i++){

        if (fscanf(input_file, "%i", &tmp_obj.color[i]) <= 0){

            printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
    }

    if (fgetc_without_space(input_file) != '}'){

        printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    }


    if (fscanf(input_file, "%f", &tmp) > 0){

        tmp_obj.transpendecy = tmp;
    } else{

        printf("Incorrect transpendency data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    }


    if (fscanf(input_file, "%f", &tmp) > 0){

        tmp_obj.surface = tmp;
    } else{

        printf("Incorrect surface data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    }


    if (fgetc_without_space(input_file) != '|'){

        fseek(input_file, -1, SEEK_CUR);

        if (fgetc_without_space(input_file) == '('){

            int counter = 0;
            char c = '\0';

            while (((c = fgetc_without_space(input_file)) != ')') && (c != '|') && (c != EOF) && (c != '\n')){
                
                fseek(input_file, -1, SEEK_CUR);

                if (fscanf(input_file, "%f", &tmp) > 0){

                    counter++;
                    tmp_obj.phys_param = (float*)realloc(tmp_obj.phys_param, counter * sizeof(float));
                    tmp_obj.phys_param[counter - 1] = tmp;
                } else{

                    break;
                }
            }
            
            tmp_obj.number_of_param = counter;

            if (c != ')'){

                printf("Incorrect physical fields data in file[%ld](can't find ')')\n", ftell(input_file));
                go_to_end_of_line(input_file);
            }
        } else{

            printf("Incorrect physical fields data in file[%ld](can't find '(')\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
    }
}

