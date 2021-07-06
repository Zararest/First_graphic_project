#include "headers/Objects.h"

Object::Object(){

    color = (int*)calloc(3, sizeof(int));

    transpendecy = 0;   //прозрачность
    surface = 0;        //параметр рассеивания при отражении
    color[0] = 228;
    color[1] = 228;
    color[2] = 228;
    number_of_param = 0; 
}

Object::~Object(){

    //printf("destructor obj\n");
    transpendecy = 0; 
    surface = 0;        
    number_of_param = 0; 

    free(color);
}

void get_object_params(FILE* input_file, Object& tmp_obj){

    float tmp = 0; 

    if (my_getc(input_file) != '{'){

        printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    } else{

        fgetc(input_file);
    }    

    for (int i = 0; i < 3; i++){

        if (fscanf(input_file, "%i", &tmp_obj.color[i]) <= 0){

            printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
    }

    if (my_getc(input_file) != '}'){

        printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    } else{

        fgetc(input_file);
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


    if (my_getc(input_file) == '|'){

        fgetc(input_file);
    } else{

        if (fgetc(input_file) == '('){

            int counter = 0;
            char c = '\0';

            while (((c = my_getc(input_file)) != ')') && (c != '|') && (c != '\n')){
                
                if (((c >= '0') && (c <= '9')) || (c == '-')){

                    fseek(input_file, -1, SEEK_CUR);
                }

                if (fscanf(input_file, "%f", &tmp) > 0){

                    counter++;
                    tmp_obj.phys_param = (float*)realloc(tmp_obj.phys_param, counter * sizeof(float));
                    tmp_obj.phys_param[counter - 1] = tmp;
                } else{

                    break;
                }
            }
            
            tmp_obj.number_of_param = counter;

            my_getc(input_file);

            if (my_getc(input_file) != '|'){

                printf("Too many parameters in file[%ld]\n", ftell(input_file));
                go_to_end_of_line(input_file);

                tmp_obj.number_of_param = 0;
            }
            fgetc(input_file);

        } else{

            printf("Incorrect physical fields data in file[%ld]\n", ftell(input_file));
        }
    }

    
}


void go_to_end_of_line(FILE* file){

    int counter = 0;
    char c = '\0';

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