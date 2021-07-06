#include "headers/Objects.h"

Sphere::Sphere(FILE* input_file): 

    Object(),
    centre(),    
    velocity(3, 1, (float*)NULL)
{

    radius = 1;
    mass = 0;
    phys_param = NULL;
    dissipation = 0;

    if (input_file != NULL){

        float vector_data[3] = {0, 0, 0};
        float tmp = 0;
        char c = '\0';

        if (my_getc(input_file) != '{'){

            printf("Incorrect centre data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            c = fgetc(input_file);
            
            if (c != '{'){

                fseek(input_file, -1, SEEK_CUR);
            }
        }

        for (int i = 0; i < 3; i++){

            if (fscanf(input_file, "%f", &vector_data[i]) <= 0){

                printf("Incorrect centre data in file[%ld]\n", ftell(input_file));
                go_to_end_of_line(input_file);
            }
        }
        centre.x = vector_data[0];
        centre.y = vector_data[1];
        centre.z = vector_data[2];


        if (my_getc(input_file) != '}'){

            printf("Incorrect radius data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            fgetc(input_file);
        }


        if (fscanf(input_file, "%f", &tmp) > 0){

            radius = tmp;
        } else{

            printf("Incorrect radius data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }


        if (fscanf(input_file, "%f", &tmp) > 0){

            mass = tmp;
        } else{

            printf("Incorrect mass data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }


        if (my_getc(input_file) != '{'){

            printf("Incorrect velocity data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            fgetc(input_file);
        }

        for (int i = 0; i < 3; i++){

            if (fscanf(input_file, "%f", &vector_data[i]) <= 0){

                printf("Incorrect velocity data in file[%ld]\n", ftell(input_file));
                go_to_end_of_line(input_file);
            }
        }
        velocity[0][0] = vector_data[0];
        velocity[1][0] = vector_data[1];
        velocity[2][0] = vector_data[2];

        if (my_getc(input_file) != '}'){

            printf("Incorrect velocity data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            fgetc(input_file);
        }

        if (fscanf(input_file, "%f", &tmp) > 0){

            dissipation = tmp;
        } else{

            printf("Incorrect dissipation data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }


        if (my_getc(input_file) != '{'){

            printf("Incorrect colors data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            fgetc(input_file);
        }    

        for (int i = 0; i < 3; i++){

            if (fscanf(input_file, "%i", &color[i]) <= 0){

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

            transpendecy = tmp;
        } else{

            printf("Incorrect transpendency data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }


        if (fscanf(input_file, "%f", &tmp) > 0){

            surface = tmp;
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
                        phys_param = (float*)realloc(phys_param, counter * sizeof(float));
                        phys_param[counter - 1] = tmp;
                    } else{

                        break;
                    }
                }
                
                number_of_param = counter;

                my_getc(input_file);

                if (my_getc(input_file) != '|'){

                    printf("Too many parameters in file[%ld]\n", ftell(input_file));
                    go_to_end_of_line(input_file);

                    number_of_param = 0;
                }
                fgetc(input_file);

            } else{

                printf("Incorrect physical fields data in file[%ld]\n", ftell(input_file));
            }
        }
        
        if (my_getc(input_file) != '\n'){

            printf("Too many parameters in file[%ld]\n", ftell(input_file));
        }
        fgetc(input_file);
    } 
}

Sphere::Sphere(Sphere&& old_obj):

    velocity(old_obj.velocity),//какой тут конструктор вызывается
    centre(old_obj.centre)
{   
    free(phys_param);
    radius = old_obj.radius;
    mass = old_obj.mass;
    dissipation = old_obj.dissipation;
    phys_param = old_obj.phys_param;

    old_obj.phys_param = NULL;
    old_obj.radius = 0;
    old_obj.mass = 0;
    old_obj.dissipation = 0;
}

Sphere::~Sphere(){

    radius = 0;
    mass = 0;
    dissipation = 0;

    free(phys_param);
}

Sphere& Sphere::operator = (Sphere&& rv){

    free(color);
    transpendecy = rv.transpendecy;
    surface = rv.surface;
    color = rv.color;
    number_of_param = rv.number_of_param;

    free(phys_param);
    centre = rv.centre;
    velocity = rv.velocity;
    radius = rv.radius;
    mass = rv.mass;
    dissipation = rv.dissipation;
    phys_param = rv.phys_param;

    rv.color = NULL;
    rv.phys_param = NULL;

    return *this;
}

void Sphere::get_info(FILE* output_file){

    fprintf(output_file, "Sphere_info:\n\n");

    fprintf(output_file, "1) Illumination data:\n");
    fprintf(output_file, "\ttranspendecy = {%lf}\n", transpendecy);
    fprintf(output_file, "\tsurface = {%lf}\n", surface);
    fprintf(output_file, "\tcolor = {%i, %i, %i}\n", color[0], color[1], color[2]);

    fprintf(output_file, "2) Sphere data:\n\n");
    fprintf(output_file, "\tnumber of physical parameters = {%i}\n", number_of_param);
    fprintf(output_file, "\tcentre = {%f, %f, %f}\n", centre.x, centre.y, centre.z);
    fprintf(output_file, "\tradius = {%f}\n", radius);
    fprintf(output_file, "\tmass = {%f}\n", mass);
    fprintf(output_file, "\tvelocity = {%f, %f, %f}\n", velocity[0][0], velocity[1][0], velocity[2][0]);
    fprintf(output_file, "\tdissipation = {%lf}\n", dissipation);

    fprintf(output_file, "3) Physical parameters:\n");

    if (number_of_param == 0){

        fprintf(output_file, "-----\n");
    } else{

        fprintf(output_file, "\n");

        for (int i = 0; i < number_of_param; i++){

            fprintf(output_file, "|%f| ", phys_param[i]);
        }
        fprintf(output_file, "\n");
    }
    fprintf(output_file, "\n");
}