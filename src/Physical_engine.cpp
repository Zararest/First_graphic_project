#include "headers/Physical_space.h"

Physics_engine::Physics_engine(Space& space):

    cur_space(space)
{
    spheres_that_bumped = (int*)calloc(cur_space.number_of_spheres, sizeof(int));
}

Physics_engine::~Physics_engine(){

    free(spheres_that_bumped);
}

float get_time_before_bump(Sphere& fir_sphere, Sphere& sec_sphere){

    float oncoming_speed = 0;
    Matrix tmp_axis(3, 1, (float)0);
    
    tmp_axis = create_vector(fir_sphere.centre, sec_sphere.centre);

    if (tmp_axis.modul() == 0){

        return 0;
    }

    oncoming_speed = projection(fir_sphere.velocity, tmp_axis) - projection(sec_sphere.velocity, tmp_axis);

    if (oncoming_speed <= 0){

        return 0;
    }

    return (distance(fir_sphere.centre, sec_sphere.centre) - fir_sphere.radius - sec_sphere.radius) / oncoming_speed;
}

float get_time_before_bump(Flatness& flat, Sphere& sphere){

    float oncoming_speed = 0, koef_D = (-1) * get_koef_D(flat.base_point, flat);

    if (sphere.mass == 0){

        return 0;
    }

    oncoming_speed = projection(sphere.velocity, flat.normal) - projection(flat.velocity, flat.normal);

    if ((get_koef_D(sphere.centre, flat) + koef_D) < 0){

        oncoming_speed *= -1;
    }  

    if ((oncoming_speed <= 0)){// || (check_intersection_with_bound_points(flat, sphere.velocity, sphere.centre) == 0)){

        return 0;
    } 

    return (distance(flat, sphere.centre) - sphere.radius) / oncoming_speed;
}

void calculate_bump(Sphere& fir_sphere, Sphere& sec_sphere){//шары абсолютно гладкие и при ударе никуда не двигаются 

    float oncoming_speed = 0, discriminant = 0;
    Matrix tmp_axis(3, 1, (float)0), tmp(3, 1, (float)0);
    
    tmp_axis = create_vector(fir_sphere.centre, sec_sphere.centre);

    if (tmp_axis.modul() != 0){//расчеты сфотканы на телефон

        oncoming_speed = projection(fir_sphere.velocity, tmp_axis) - projection(sec_sphere.velocity, tmp_axis);
        assert(oncoming_speed > 0);

        float m_L = fir_sphere.mass, m_R = sec_sphere.mass;
        float V_L = projection(fir_sphere.velocity, tmp_axis), V_R = projection(sec_sphere.velocity, tmp_axis);
        float U_L = 0, U_R = 0, tmp_const_a = 0, tmp_const_b = 0;
        float dissip = (fir_sphere.dissipation + sec_sphere.dissipation) / 2;
        
        tmp_const_a = V_L * m_L + V_R * m_R;
        tmp_const_b = dissip * (V_L * V_L * m_L + V_R * V_R * m_R);
        discriminant = 4 * tmp_const_a * tmp_const_a - 4 * (m_R * m_R / m_L + m_R) * (tmp_const_a * tmp_const_a / m_L - tmp_const_b);
        
        if (discriminant >= 0){

            U_R = (sqrt(discriminant) + 2 * tmp_const_a * m_R / m_L) / (2 * (m_R * m_R / m_L + m_R));
            U_L = tmp_const_a / m_L - U_R * m_R / m_L;
            oncoming_speed = U_L - U_R;

            printf("first speed = %f secnd speed = %f\n\n", U_L, U_R);
            if (oncoming_speed > 0){

                U_R = ((-1) * sqrt(discriminant) + 2 * tmp_const_a * m_R / m_L) / (2 * (m_R * m_R / m_L + m_R));
                U_L = tmp_const_a / m_L - U_R * m_R / m_L;
                oncoming_speed = U_L - U_R;

                printf("another first speed = %f secnd speed = %f\n\n", U_L, U_R);
            }

            if (oncoming_speed < 0){

                fir_sphere.velocity = fir_sphere.velocity + (U_L - V_L) / (tmp_axis.modul()) * tmp_axis;
                sec_sphere.velocity = sec_sphere.velocity + (U_R - V_R) / (tmp_axis.modul()) * tmp_axis;
            } else{

                printf("Can't calculate sphere bump(incorrect resulting velocity)\n");
                fir_sphere.get_info(stdout);
                sec_sphere.get_info(stdout);
            }

        } else{

            printf("Can't calculate sphere bump(discriminant < 0)\n");
            fir_sphere.get_info(stdout);
            sec_sphere.get_info(stdout);
        }
    } else{

        printf("Can't calculate sphere bump(spheres too close)\n");
        fir_sphere.get_info(stdout);
        sec_sphere.get_info(stdout);
    }

    
}

void calculate_bump(Flatness& flat, Sphere& sphere){

    float projection_normal = 0, koef_D = (-1) * get_koef_D(flat.base_point, flat);;
    Matrix related_speed(3, 1, (float)0);
    related_speed = sphere.velocity - flat.velocity;

    projection_normal = projection(related_speed, flat.normal);

    assert(flat.normal.modul() > 0);

    related_speed = sphere.dissipation * (related_speed - 2 * (projection_normal) / flat.normal.modul() * flat.normal);

    sphere.velocity = flat.velocity + related_speed;

    //flat.get_info(stdout);
    //sphere.get_info(stdout);
}

void Physics_engine::call_collision_engine(float delta_t){

    float tmp = 0, min_sphere = 0, min_flat = 0;
    int number_of_nearest_sphere = 0, number_of_nearest_flat = 0, first_iter = 0;

    for (int j = 0; j < cur_space.number_of_spheres; j++){

        first_iter = 1;

        for (int i = 0; i < cur_space.number_of_spheres; i++){

            if (i != j){

                tmp = get_time_before_bump(cur_space.array_of_spheres[i], cur_space.array_of_spheres[j]);

                if (first_iter == 1){

                    min_sphere = tmp;
                    number_of_nearest_sphere = i;
                    first_iter = 0;
                }

                if (tmp < min_sphere){

                    min_sphere = tmp;
                    number_of_nearest_sphere = i;
                }
            }
        }

        for (int i = 0; i < cur_space.number_of_flats; i++){

            tmp = get_time_before_bump(cur_space.array_of_flats[i], cur_space.array_of_spheres[j]);

            if ((min_flat == 0) && (tmp != 0)){

                number_of_nearest_flat = i;
                min_flat = tmp;
            }

            if ((tmp < min_flat) && (tmp != 0)){

                number_of_nearest_flat = i;
                min_flat = tmp;
            }
        }

        if ((min_flat <= min_sphere) && (cur_space.number_of_flats != 0) && (min_flat <= delta_t)){

            if (spheres_that_bumped[j] == 0){

                calculate_bump(cur_space.array_of_flats[number_of_nearest_flat], cur_space.array_of_spheres[j]);
                spheres_that_bumped[j] = 1;
            }
        }

        if (((min_sphere < min_flat) || (cur_space.number_of_flats != 0)) && (first_iter == 0)){
            
            if (min_sphere <= delta_t){
                
                if ((spheres_that_bumped[j] == 0) && ((spheres_that_bumped[number_of_nearest_sphere] == 0))){

                    calculate_bump(cur_space.array_of_spheres[j], cur_space.array_of_spheres[number_of_nearest_sphere]);//обработать нулевое время
                    spheres_that_bumped[j] = 1;
                    spheres_that_bumped[number_of_nearest_sphere] = 1;
                } 
            }
        }
    }
}

float min(float left_num, float right_num){

    if (left_num <= right_num){

        return left_num;
    } else{

        return right_num;
    }
}

Matrix Physics_engine::get_force(Sphere& cur_sphere){

    Matrix result_force(3, 1, (float)0);

    for (int i = 0; i < cur_space.number_of_elastic_inter; i++){

        if (cur_sphere == *cur_space.arr_elastic_inter[i].fir_obj){

            result_force = result_force + cur_space.arr_elastic_inter[i].cur_force_fir_sphere();
        }

        if (cur_sphere == *cur_space.arr_elastic_inter[i].sec_obj){

            result_force = result_force + cur_space.arr_elastic_inter[i].cur_force_sec_sphere();
        }
    }

    if (cur_sphere.number_of_param > cur_space.number_of_fields){

        printf("Too many phys parameters\n");
    }

    for (int i = 0; i < min(cur_sphere.number_of_param, cur_space.number_of_fields); i++){

        result_force = result_force + cur_space.phys_fields[i].return_force(cur_sphere);
    }

    return result_force;
}

void Physics_engine::call_phys_engine(float delta_t){

    float tmp = 0;
    for (int i = 0; i < cur_space.number_of_spheres; i++){

        if (cur_space.array_of_spheres[i].mass != 0){

            tmp = 0.5 * delta_t / cur_space.array_of_spheres[i].mass;
            cur_space.array_of_spheres[i].velocity = cur_space.array_of_spheres[i].velocity + tmp * get_force(cur_space.array_of_spheres[i]);
        }
    }
    
    call_collision_engine(delta_t);

    for (int i = 0; i < cur_space.number_of_spheres; i++){

        if (spheres_that_bumped[i] == 0){

            if (cur_space.array_of_spheres[i].mass != -1){

                cur_space.array_of_spheres[i].shift(delta_t);
            }
        } else{

            spheres_that_bumped[i] = 0;
        }
    }

    for (int i = 0; i < cur_space.number_of_flats; i++){

        cur_space.array_of_flats[i].shift(delta_t);
    }
}

