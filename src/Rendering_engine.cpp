#include "headers/Physical_space.h"

#define VERTICAL_WINDOW_SIZE 700
#define HORIZONTAL_WINDOW_SIZE 1200
#define NUMBER_OF_PIXELS_IN_SIGLE_SEGMENT 10


int get_light_sources(FILE* input_file, Light_source*& arr_of_sources){

    int counter = 0;

    while (fgetc_without_space(input_file) != EOF){

        counter++;
        fseek(input_file, -1, SEEK_CUR);

        arr_of_sources = (Light_source*)realloc(arr_of_sources, counter * sizeof(Light_source));
        arr_of_sources[counter - 1] = Light_source(input_file);
        arr_of_sources[counter - 1].get_info(stdout);
    }

    return counter;
}

Rendering_engine::Rendering_engine(Space& space, FILE* input_file, Matrix&  normal, Point& position, float observ_angle):

    screen_normal(normal),
    screen_axis_x(3, 1, (float)0),
    screen_axis_y(3, 1, (float)0),
    observ_position(position),
    cur_space(space),
    window(sf::VideoMode(HORIZONTAL_WINDOW_SIZE, VERTICAL_WINDOW_SIZE), "UwU"),
    screen(),
    sprite()
{

    float angle_in_rad = observ_angle * 3.1415 / 180;

    number_of_light_sources = 0;
    arr_of_light_sources = NULL;
    vertical_window_size = VERTICAL_WINDOW_SIZE;
    horizontal_window_size = HORIZONTAL_WINDOW_SIZE;
    number_of_pixels_in_single_segment = NUMBER_OF_PIXELS_IN_SIGLE_SEGMENT;

    screen_axis_x[1][0] =  1 / number_of_pixels_in_single_segment;
    screen_axis_y[2][0] = -1 / number_of_pixels_in_single_segment;
    screen_normal.normalize();

    assert(tan(angle_in_rad /  2) != 0);
    distance_to_screen = horizontal_window_size / (2 * tan(angle_in_rad /  2)) / number_of_pixels_in_single_segment;

    if (input_file != 0){

        number_of_light_sources = get_light_sources(input_file, arr_of_light_sources);
    }
    
    assert(number_of_light_sources != 0);

    screen.create(horizontal_window_size, vertical_window_size);
    sprite.setTexture(screen, true);
    arr_of_pixels = new sf::Uint8[vertical_window_size * horizontal_window_size * 4];
}

Rendering_engine::~Rendering_engine(){

    if (window.isOpen()){

        window.close();
    }

    free(arr_of_light_sources);
}

void Rendering_engine::set_pixel(int horizontal_position, int vertical_position, sf::Color pixel_color){

    //printf("position = %i\n", 1);
    //arr_of_pixels[vertical_position * horizontal_window_size + horizontal_position + 0] = 0;
    //printf("here4\n");

    arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 0] = pixel_color.r;
    arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 1] = pixel_color.g;
    arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 2] = pixel_color.b;
    arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 3] = pixel_color.a;

    if (vertical_window_size / 2 == vertical_position){

        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 0] = 255;
        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 1] = 255;
        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 2] = 255;
        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 3] = 255;
    }

    if (horizontal_window_size / 2 == horizontal_position){

        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 0] = 255;
        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 1] = 255;
        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 2] = 255;
        arr_of_pixels[(vertical_position * horizontal_window_size + horizontal_position) * 4 + 3] = 255;
    }
}

Light_source Rendering_engine::check_ray_of_light_intersec(Light_source& cur_light_source, Point& final_point){

    Light_source tmp_light(cur_light_source);
    Matrix ray_of_light(3, 1, (float)0);
    Point cur_intersec_point;
    float distance_to_final_point = distance(cur_light_source.position, final_point);

    ray_of_light = create_vector(cur_light_source.position, final_point);

    for (int i = 0; i < cur_space.number_of_spheres; i++){

        if (cur_space.array_of_spheres[i].check_intersection(ray_of_light, final_point) == 1){

            cur_intersec_point = cur_space.array_of_spheres[i].get_intersection_point(ray_of_light, final_point);

            if (distance(cur_intersec_point, cur_light_source.position) < distance_to_final_point * 0.98){ //тут траблы с точностью

                tmp_light.brightness -= cur_space.array_of_spheres[i].color.a / 255;
            }
        }
    }

    if (tmp_light.brightness < 0){

        tmp_light.brightness = 0;
    }
    
    return tmp_light;
}

sf::Color Rendering_engine::get_pixel_color(Matrix& vector_through_pixel, Point& base_point_of_vector){

    int obj_has_found = 1, fir_iteration = 1;
    float light_power = 1, distance_to_prev_object = 0, min_distance = 0, distance_to_obj = 0;
    Sphere* point_to_sph = NULL;
    Light_source tmp_light(NULL);
    Point intersec_point;
    sf::Color pixel_color, prev_color;

    pixel_color.a = 0;
    pixel_color.r = 0;
    pixel_color.b = 0;
    pixel_color.g = 0;

    while ((pixel_color.a < 255) && (obj_has_found != 0)){

        fir_iteration = 1;
        point_to_sph = NULL;
        obj_has_found = 0;

        for (int i = 0; i < cur_space.number_of_spheres; i++){

            if (cur_space.array_of_spheres[i].check_intersection(vector_through_pixel, base_point_of_vector) == 1){

                obj_has_found = 1;
                intersec_point = cur_space.array_of_spheres[i].get_intersection_point(vector_through_pixel, base_point_of_vector);

                distance_to_obj = distance(intersec_point, base_point_of_vector);
                
                if (fir_iteration == 1){

                    fir_iteration = 0;
                    min_distance = distance_to_obj;
                    point_to_sph = &cur_space.array_of_spheres[i];
                }

                if ((distance_to_obj > distance_to_prev_object) && (distance_to_obj < min_distance)){
                    
                    min_distance = distance_to_obj;
                    point_to_sph = &cur_space.array_of_spheres[i];
                }
            }
        }

        if (point_to_sph != NULL){

            for (int j = 0; j < number_of_light_sources; j++){
                
                tmp_light = check_ray_of_light_intersec(arr_of_light_sources[j], intersec_point);
                pixel_color += point_to_sph->illumination(vector_through_pixel, tmp_light, intersec_point);
            }
        }
    }

    return pixel_color;
}

void Rendering_engine::show_window(){

    sf::Color pixel_color;
    Matrix vector_through_pixel(3, 1, (float)0);

    /*Matrix vector(3, 1, (float)0);
    Point base_point;

    vector[1][0] = 1;
    vector[2][0] = 1;

    cur_space.array_of_spheres[0].get_intersection_point(vector, base_point).print(stdout);
    printf("\n|%i|\n", cur_space.array_of_spheres[0].check_intersection(vector, base_point));
    fgetc(stdin);*/
    for (int i = 0; i < vertical_window_size; i++){

        for (int j = 0; j < horizontal_window_size; j++){ 

            vector_through_pixel = screen_axis_x * (j - horizontal_window_size / 2) + screen_axis_y * (i - vertical_window_size / 2) - (distance_to_screen) * screen_normal;
            
            pixel_color = this->get_pixel_color(vector_through_pixel, observ_position);
            //printf("here3\n");
            this->set_pixel(j, i, pixel_color);
        }
    }
    printf("frame\n");

    screen.update(arr_of_pixels);
    window.clear();
    window.draw(sprite);
    window.display();
}