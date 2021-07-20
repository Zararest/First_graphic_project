#include "headers/Objects.h"

Flatness::Flatness(FILE* input_file):

    Object(),
    normal(3, 1, (float)0),
    base_point(),
    velocity(3, 1, (float)0)
{
    bounding_points = NULL;
    
    if (input_file  != NULL){

        float vector_data[3] = {0, 0, 0};
        float tmp = 0;
        int counter = 0;
        char c = '\0';

        normal = get_vector(input_file);

        base_point = get_point(input_file);

        if (fgetc_without_space(input_file) != '{'){

            printf("Incorrect bounding points in file[%ld](can't find '{')\n",  ftell(input_file));
            go_to_end_of_line(input_file);
        }

        if ((c = fgetc_without_space(input_file)) == '('){

            fseek(input_file, -1, SEEK_CUR);
            counter++;
            bounding_points = (Point*)realloc(bounding_points, counter * sizeof(Point));
            bounding_points[counter - 1] = get_point(input_file);

            while (((c = fgetc_without_space(input_file)) != '}') && (c != '|') && (c != EOF) && (c != '\n')){

                fseek(input_file, -1, SEEK_CUR);
                counter++;
                bounding_points = (Point*)realloc(bounding_points, counter * sizeof(Point));
                bounding_points[counter - 1] = get_point(input_file);
            }

            bounding_points = (Point*)realloc(bounding_points, (counter + 1) * sizeof(Point));
            bounding_points[counter] = bounding_points[0];

            if (counter < 3){

                printf("Warning: too few bounding points in file[%ld]\n", ftell(input_file));
            }

            if (c != '}'){

                printf("Incorrect bounding points in file[%ld](can't find '}')\n",  ftell(input_file));
                go_to_end_of_line(input_file);
            }
        } else{

            if (c != '}'){
                
                printf("Incorrect bounding points in file[%ld](can't find '}')\n",  ftell(input_file));
                go_to_end_of_line(input_file);
            }
        }

        velocity = get_vector(input_file);

        get_object_params(input_file, *this); 

        if (fgetc_without_space(input_file) != '|'){

            printf("Too many parameters in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
            fgetc(input_file);
        }
        fgetc_without_space(input_file);
    }
}

Flatness::Flatness(Flatness&& old_obj):

    velocity(old_obj.velocity),
    normal(old_obj.normal),
    base_point(old_obj.base_point)
{
    free(phys_param);
    free(color);
    free(bounding_points);
    phys_param = old_obj.phys_param;
    color = old_obj.color;
    bounding_points = old_obj.bounding_points;

    old_obj.phys_param = NULL;
    old_obj.bounding_points = NULL;
    old_obj.color = NULL;

    transpendecy = old_obj.transpendecy;
    surface = old_obj.surface;
    number_of_param = old_obj.number_of_param;
}

Flatness::~Flatness(){

    //printf("Деструктор flatness\n");
}

void Flatness::shift(float delta_t){

    int i = 1;

    base_point.x += velocity[0][0] * delta_t;
    base_point.y += velocity[1][0] * delta_t;
    base_point.z += velocity[2][0] * delta_t;

    if (bounding_points != NULL){

        while (bounding_points[i] != bounding_points[0]){
            
            bounding_points[i].x += velocity[0][0] * delta_t;
            bounding_points[i].y += velocity[1][0] * delta_t;
            bounding_points[i].z += velocity[2][0] * delta_t;
            i++;
        }

        bounding_points[0].x += velocity[0][0] * delta_t;
        bounding_points[0].y += velocity[1][0] * delta_t;
        bounding_points[0].z += velocity[2][0] * delta_t;

        bounding_points[i] = bounding_points[0];
    }
}

Flatness& Flatness::operator = (Flatness&& rv){

    free(color);
    transpendecy = rv.transpendecy;
    surface = rv.surface;
    color = rv.color;
    number_of_param = rv.number_of_param;

    free(phys_param);
    phys_param = rv.phys_param;
    normal = rv.normal;
    base_point = rv.base_point;

    free(bounding_points);
    bounding_points = rv.bounding_points;
    velocity = rv.velocity;

    rv.phys_param = NULL;
    rv.color = NULL;
    rv.bounding_points = NULL;

    return *this;
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

void Flatness::init(){

    normal.init();
    base_point.x = 0;
    base_point.y = 0;
    base_point.z = 0;
    bounding_points = NULL;
    velocity.init();
}

