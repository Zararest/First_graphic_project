#include "headers/Objects.h"

Point get_base_point(FILE* input_file){

    float tmp[3] = {0, 0, 0};
    Point tmp_obj;

    for (int i = 0; i < 3; i++){

        if (fscanf(input_file, "%f", &tmp[i]) <= 0){

            printf("Incorrect bounding 4 points in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
    }

    if (my_getc(input_file) != ')'){

        printf("Incorrect bounding points 5 in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
    }

    if (fgetc(input_file) != ')'){

        fseek(input_file, -1, SEEK_CUR);
    }

    tmp_obj.x = tmp[0];
    tmp_obj.y = tmp[1];
    tmp_obj.z = tmp[2];

    return static_cast<Point&&>(tmp_obj);
}

Flatness::Flatness(FILE* input_file):

    Object(),
    normal(3, 1, (float)0),
    base_point(),
    velocity(3, 1, (float)0)
{
    bounding_points = NULL;
    phys_param = NULL;

    if (input_file  != NULL){

        float vector_data[3] = {0, 0, 0};
        float tmp = 0;
        int counter = 0;
        char c = '\0';

        if (my_getc(input_file) != '{'){

            printf("Incorrect normal vector data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            c = fgetc(input_file);
            
            if (c != '{'){

                fseek(input_file, -1, SEEK_CUR);
            }
        }

        for (int i = 0; i < 3; i++){

            if (fscanf(input_file, "%f", &vector_data[i]) <= 0){

                printf("Incorrect normal vector data in file[%ld]\n", ftell(input_file));
                go_to_end_of_line(input_file);
            }
        }
        normal[0][0] = vector_data[0];
        normal[1][0] = vector_data[1];
        normal[2][0] = vector_data[2];

        if (my_getc(input_file) != '}'){

            printf("Incorrect normal vector data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            fgetc(input_file);
        }


        if (my_getc(input_file) != '{'){

            printf("Incorrect point data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            c = fgetc(input_file);
            
            if (c != '{'){

                fseek(input_file, -1, SEEK_CUR);
            }
        }

        for (int i = 0; i < 3; i++){

            if (fscanf(input_file, "%f", &vector_data[i]) <= 0){

                printf("Incorrect point data in file[%ld]\n", ftell(input_file));
                go_to_end_of_line(input_file);
            }
        }
        base_point.x = vector_data[0];
        base_point.y = vector_data[1];
        base_point.z = vector_data[2];

        if (my_getc(input_file) != '}'){

            printf("Incorrect point data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            fgetc(input_file);
        }


        if (my_getc(input_file) != '{'){

            printf("Incorrect bounding points 1 in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        } else{

            c = fgetc(input_file);
            
            if (c != '{'){

                fseek(input_file, -1, SEEK_CUR);
            }
        }

        while (((c = my_getc(input_file)) != '}') && (c != '|') && (c != '\n')){
            
            c = fgetc(input_file);

            if (c != '('){

                fseek(input_file, -1, SEEK_CUR);
            }

            if (c == '('){

                counter++;
                bounding_points = (Point*)realloc(bounding_points, counter * sizeof(Point));
                bounding_points[counter - 1] = get_base_point(input_file);
            } else{

                printf("Incorrect bounding 2 points in file[%ld]\n", ftell(input_file));
                go_to_end_of_line(input_file);
            }
        }

        if (counter != 0){

            bounding_points = (Point*)realloc(bounding_points, (counter + 1) * sizeof(Point));
            bounding_points[counter] = bounding_points[0];

            if (counter < 3){

                printf("Warning: too few bounding points in file[%ld]\n", ftell(input_file));
            }
        }
        
        if ((c = my_getc(input_file)) != '}'){

            printf("my_getc = %c\n", c);
            printf("Incorrect bounding 3 points in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }

        c = fgetc(input_file);

        if (c != '}'){

            fseek(input_file, -1, SEEK_CUR);
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


        get_object_params(input_file, *this);     

        if (my_getc(input_file) != '\n'){

            printf("Too many parameters in file[%ld]\n", ftell(input_file));
        }
        fgetc(input_file);
    }
    printf("end\n");
}

void Flatness::get_info(FILE* output_file){

    fprintf(output_file, "Flatness info:\n\n");

    fprintf(output_file, "1) Illumination data:\n");
    fprintf(output_file, "\ttranspendecy = {%lf}\n", transpendecy);
    fprintf(output_file, "\tsurface = {%lf}\n", surface);
    fprintf(output_file, "\tcolor = {%i, %i, %i}\n", color[0], color[1], color[2]);

    fprintf(output_file, "2) Flatness data\n\n");
    fprintf(output_file, "\tnormal = {%f, %f, %f}\n", normal[0][0], normal[1][0], normal[2][0]);
    fprintf(output_file, "\tbase point = {%f, %f, %f}\n", base_point.x, base_point.y, base_point.z);
    fprintf(output_file, "\tbounding points:\n");

    if (bounding_points != NULL){

        int i = 1;

        fprintf(output_file, "\t");
        bounding_points[0].print(output_file);
        fprintf(output_file, "\n");

        while (bounding_points[i] != bounding_points[0]){
           
            fprintf(output_file, "\t");
            bounding_points[i].print(output_file);
            fprintf(output_file, "\n");
            i++;
        }
    } else{

        fprintf(output_file, "-----\n");
    }

    fprintf(output_file, "\tvelocity = {%f, %f, %f}\n", velocity[0][0], velocity[1][0], velocity[2][0]);

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

Flatness::~Flatness(){


}


