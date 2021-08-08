#include "headers/Physical_space.h"

Light_source::Light_source(FILE* input_file):

    position()
{
    
    brightness = 1;
    
    if (input_file != NULL){

        float tmp = 0;

        position = get_point(input_file);

        if (fscanf(input_file, "%f", &tmp) > 0){

            brightness = tmp;
        } else{

            printf("Incorrect brightness data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }

        if (fgetc_without_space(input_file) != '|'){

            printf("Too many parameters in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
        fgetc_without_space(input_file);
    }
}

Light_source::Light_source(const Light_source& old_obj){

    position = old_obj.position;
    brightness = old_obj.brightness;
}

Light_source::Light_source(Light_source&& rv){

    position = rv.position;
    brightness = rv.brightness;
}

Light_source::~Light_source(){

}

Light_source& Light_source::operator = (Light_source&& rv){

    position = rv.position;
    brightness = rv.brightness;
    
    return *this;
}

void Light_source::get_info(FILE* output_file){

    fprintf(output_file, "Light source info:\n\n");

    fprintf(output_file, "\t1) Position");
    position.print(output_file);
    fprintf(output_file, "\n\t2) Brightness %f\n", brightness);
    fprintf(output_file, "\n");
}

