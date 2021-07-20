#include "headers/Objects.h"

Sphere::Sphere(FILE* input_file): 

    Object(),
    centre(),    
    velocity(3, 1, (float)0)
{
    radius = 1;
    mass = 0;
    phys_param = NULL;
    dissipation = 0;

    if (input_file != NULL){

        float tmp = 0;
        char c = '\0';

        centre = get_point(input_file);

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

        velocity = get_vector(input_file);

        if (fscanf(input_file, "%f", &tmp) > 0){

            if ((dissipation <= 1) && (dissipation >= 0)){

                dissipation = tmp;
            } else{

                printf("Strange dissipation value in file[%ld](dissipation value is '1' now)\n", ftell(input_file));
                dissipation = 1;
            }
            
        } else{

            printf("Incorrect dissipation data in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }

        get_object_params(input_file, *this); 

        if (fgetc_without_space(input_file) != '|'){

            printf("Too many parameters in file[%ld]\n", ftell(input_file));
            go_to_end_of_line(input_file);
        }
        fgetc_without_space(input_file);
    }
}

Sphere::Sphere(Sphere&& old_obj):

    velocity(old_obj.velocity),//какой тут конструктор вызывается
    centre(old_obj.centre)
{   
    printf("конструктор перемещения сферы\n");
    free(phys_param);
    free(color);
    radius = old_obj.radius;
    mass = old_obj.mass;
    dissipation = old_obj.dissipation;
    phys_param = old_obj.phys_param;
    color = old_obj.color;
    transpendecy = old_obj.transpendecy;
    surface = old_obj.surface;
    number_of_param = old_obj.number_of_param;


    old_obj.color = NULL;
    old_obj.phys_param = NULL;
    old_obj.radius = 0;
    old_obj.mass = 0;
    old_obj.dissipation = 0;
}

Sphere::~Sphere(){

    radius = 0;
    mass = 0;
    dissipation = 0;
}

void Sphere::shift(float delta_t){

    //printf()
    if (mass >= 0){

        centre.x += velocity[0][0] * delta_t;
        centre.y += velocity[1][0] * delta_t;
        centre.z += velocity[2][0] * delta_t;
    }
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

int operator == (const Sphere& fir_sph, const Sphere& sec_sph){

    if (fir_sph.centre != sec_sph.centre){

        return 0;
    }

    return 1;
}

int check_intersection_with_sphere(Sphere& sphere, Matrix& vector, Point& base_point_of_vector){//расчет в тетради 

    float tmp_koef_b = base_point_of_vector.x * vector[0][0] + base_point_of_vector.y * vector[1][0] + base_point_of_vector.z * vector[2][0];
    float tmp_koef_a = vector.modul() * vector.modul();
    float tmp_koef_c = pow(base_point_of_vector.x, 2) + pow(base_point_of_vector.y, 2) + pow(base_point_of_vector.z, 2);
    float determinant = 4 * tmp_koef_b * tmp_koef_b - 4 * tmp_koef_a * tmp_koef_c;

    if (determinant >= 0){

        return 1;
    } else{

        return 0;
    }
    /*float t_fir = 0, t_sec = 0;
    Point fir_intersec_point, sec_intersec_point;

    t_fir = (sqrt(determinant) - 2 * tmp_koef_b) / (2 * tmp_koef_c);
    t_sec = ((-1) * sqrt(determinant) - 2 * tmp_koef_b) / (2 * tmp_koef_c);

    fir_intersec_point.x = base_point_of_vector.x + vector[0][0] * t_fir;
    fir_intersec_point.y = base_point_of_vector.y + vector[1][0] * t_fir;
    fir_intersec_point.z = base_point_of_vector.z + vector[2][0] * t_fir;

    sec_intersec_point.x = base_point_of_vector.x + vector[0][0] * t_sec;
    sec_intersec_point.y = base_point_of_vector.y + vector[1][0] * t_sec;
    sec_intersec_point.z = base_point_of_vector.z + vector[2][0] * t_sec;

    if (distance(base_point_of_vector, fir_intersec_point) > distance(base_point_of_vector, sec_intersec_point)){

    }*/
}

void Sphere::init(){

    this->color = NULL;
    this->centre.x = 0;
    this->centre.y = 0;
    this->centre.z = 0;
    this->dissipation = 0;
    this->number_of_param = 0;
    this->phys_param = NULL;
    this->radius = 0;
    this->surface = 0;
    this->transpendecy = 0;
    this->velocity.init();
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