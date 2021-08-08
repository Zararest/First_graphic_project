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
    free(bounding_points);
    phys_param = old_obj.phys_param;
    color = old_obj.color;
    bounding_points = old_obj.bounding_points;

    old_obj.phys_param = NULL;
    old_obj.bounding_points = NULL;

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
    rv.bounding_points = NULL;

    return *this;
}

int Flatness::check_intersection(Matrix& vector, Point& base_point_of_vector){

    Point intersection_point;
    Matrix tmp_vector(3, 1, (float)0);
    float angle_sum = 0;//сумма углов между векторами с началом в точке пересечения и ограничивающими точками
    float koef_D = (-1) * get_koef_D(base_point, *this);
    float t = 0, tmp_koef_top = 0, tmp_koef_bot = 0;//параметр в уравнении прямой и числитель с знаменателем
    int i = 1;

    tmp_koef_top = koef_D + normal[0][0] * base_point_of_vector.x + normal[1][0] * base_point_of_vector.y + normal[2][0] * base_point_of_vector.z;
    tmp_koef_bot = normal[0][0] * vector[0][0] + normal[1][0] * vector[1][0] + normal[2][0] * vector[2][0];

    if (tmp_koef_bot == 0){

        return 0;
    }

    t = (-1) * tmp_koef_top / tmp_koef_bot;
    intersection_point.x = base_point_of_vector.x + vector[0][0] * t;
    intersection_point.y = base_point_of_vector.y + vector[1][0] * t;
    intersection_point.z = base_point_of_vector.z + vector[2][0] * t;

    if (bounding_points != NULL){

        while (bounding_points[0] != bounding_points[i]){

            i++;
        }

        if (i < 3){

            return 0;
        }

        tmp_vector = create_vector(intersection_point, bounding_points[0]);

        for (int j = 1; j < i; j++){

            
            angle_sum += acos(cos(tmp_vector, create_vector(intersection_point, bounding_points[j])));
            tmp_vector = create_vector(intersection_point, bounding_points[j]);
        }

        if (angle_sum >= 3.141592){

            return 1;
        } else{

            return 0;
        }
    } else{

        return 0;
    }
}

void Flatness::get_info(FILE* output_file){

    fprintf(output_file, "Flatness info:\n\n");

    fprintf(output_file, "1) Illumination data:\n");
    fprintf(output_file, "\tsurface = {%lf}\n", surface);
    fprintf(output_file, "\tcolor = {%i, %i, %i}(%i)\n", color.r, color.g, color.b, color.a);

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

