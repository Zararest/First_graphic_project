#include "headers/Physical space.h"

Elastic_interaction::Elastic_interaction(FILE* input_file, Sphere* array_of_spheres, int number_of_spheres){

    
    Point fir_tmp = get_point(input_file);
    Point sec_tmp = get_point(input_file);
    float tmp = 0;
    int correct_data = 1;

    koef_k = 0;
    normal_distance = 0;
    fir_obj = NULL;
    sec_obj = NULL;

    if (fscanf(input_file, "%f", &tmp) > 0){

        koef_k = tmp;
    } else{

        printf("Incorrect koef k data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
        correct_data = 0;
    }

    if (fscanf(input_file, "%f", &tmp) > 0){

        normal_distance = tmp;
    } else{

        printf("Incorrect normal distance data in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
        correct_data = 0;
    }
    
    if (fgetc_without_space(input_file) != '|'){

        printf("Too many parameters in file[%ld]\n", ftell(input_file));
        go_to_end_of_line(input_file);
        fgetc(input_file);
        correct_data = 0;
    }
    fgetc_without_space(input_file);

    if (correct_data != 0){

        int sphere_has_def = 0;

        for (int i = 0; i < number_of_spheres; i++){

            if (array_of_spheres[i].centre == fir_tmp){

                printf("here\n");
                sphere_has_def++;

                fir_obj = &array_of_spheres[i];
            }
        }

        if (sphere_has_def == 0){

            printf("Can't find sphere with centre");
            fir_tmp.print(stdout);
            printf("\n");
        }

        if (sphere_has_def > 1){

            printf("There are multiple sphers, the centre of which is ");
            fir_tmp.print(stdout);
            printf("\n");
        }
        
        sphere_has_def = 0;

        for (int i = 0; i < number_of_spheres; i++){

            if (array_of_spheres[i].centre == sec_tmp){

                sphere_has_def++;

                sec_obj = &array_of_spheres[i];
            }
        }

        if (sphere_has_def == 0){

            printf("Can't find sphere with centre");
            sec_tmp.print(stdout);
            printf("\n");
        }

        if (sphere_has_def > 1){

            printf("There are multiple sphers, the centre of which is ");
            sec_tmp.print(stdout);
            printf("\n");
        }
    } 
}

Elastic_interaction::Elastic_interaction(Elastic_interaction&& old_obj){

    fir_obj = old_obj.fir_obj;
    sec_obj = old_obj.sec_obj;

    koef_k = old_obj.koef_k;
    normal_distance = old_obj.normal_distance;

    old_obj.fir_obj = NULL;
    old_obj.sec_obj = NULL;
}

Elastic_interaction::~Elastic_interaction(){

    fir_obj = NULL;
    sec_obj = NULL;
    printf("Деструктор elastic\n");
}

Elastic_interaction& Elastic_interaction::operator = (Elastic_interaction&& rv){

    fir_obj = rv.fir_obj;
    sec_obj = rv.sec_obj;

    koef_k = rv.koef_k;
    normal_distance = rv.normal_distance;

    return *this;
}

void Elastic_interaction::get_info(FILE* output_file){

    fprintf(output_file, "Elastic interaction info:\n\n");
    
    fprintf(output_file, "\tcoordinates of spheres: ");
    fir_obj->centre.print(output_file);
    sec_obj->centre.print(output_file);
    fprintf(output_file, "\n");
    fprintf(output_file, "\tinteraction coefficient = %f\n", koef_k);
    fprintf(output_file, "\tnormal distance = %f\n", normal_distance);
    fprintf(output_file, "\n");
}
