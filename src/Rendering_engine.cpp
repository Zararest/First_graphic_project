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
    window(sf::VideoMode(HORIZONTAL_WINDOW_SIZE, VERTICAL_WINDOW_SIZE), "UwU")
{

    float angle_in_rad = observ_angle * 3.1415 / 180;
    printf("angle in rad %f\n", angle_in_rad);

    number_of_light_sources = 0;
    arr_of_light_sources = NULL;
    vertical_window_size = VERTICAL_WINDOW_SIZE;
    horizontal_window_size = HORIZONTAL_WINDOW_SIZE;
    number_of_pixels_in_single_segment = NUMBER_OF_PIXELS_IN_SIGLE_SEGMENT;

    screen_axis_x[1][0] = -1 / number_of_pixels_in_single_segment;
    screen_axis_y[2][0] = -1 / number_of_pixels_in_single_segment;
    screen_normal.normalize();

    assert(tan(angle_in_rad /  2) != 0);
    distance_to_screen = horizontal_window_size / (2 * tan(angle_in_rad /  2)) / number_of_pixels_in_single_segment;

    if (input_file != 0){

        number_of_light_sources = get_light_sources(input_file, arr_of_light_sources);
    }
    
    assert(number_of_light_sources != 0);
}

Rendering_engine::~Rendering_engine(){

    if (window.isOpen()){

        window.close();
    }

    free(arr_of_light_sources);
}

void Rendering_engine::set_pixel(int horizontal_position, int vertical_position, colour pixel_color){

    sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
    rectangle.move(horizontal_position, vertical_position);
    rectangle.setFillColor(sf::Color(pixel_color.R, pixel_color.G, pixel_color.B));

    window.draw(rectangle);
}

int max(int fir, int sec, int trd){

    if ((fir >= sec) && (fir >= trd)){

        return fir;
    }

    if ((sec >= fir) && (sec >= trd)){

        return sec;
    }

    if ((trd >= fir) && (trd >= sec)){

        return trd;
    } else{

        printf("dafuq\n");
        return 0;
    }
}

colour normolize(colour cur_color){

    if (max(cur_color.R, cur_color.G, cur_color.B) > 255){

        cur_color.R = cur_color.R * 255 / max(cur_color.R, cur_color.G, cur_color.B);
        cur_color.G = cur_color.G * 255 / max(cur_color.R, cur_color.G, cur_color.B);
        cur_color.B = cur_color.B * 255 / max(cur_color.R, cur_color.G, cur_color.B);

    }

    return cur_color;
}

colour Rendering_engine::get_pixel_color(Matrix& vector_through_pixel, Point& base_point_of_vector){

    float light_power = 1;
    float distance_to_prev_object = 0;
    Point intersec_point;
    colour tmp_color, pixel_color;

    pixel_color.R = 0;
    pixel_color.G = 0;
    pixel_color.B = 0;

    for (int i = 0; i < cur_space.number_of_spheres; i++){
        
        if (cur_space.array_of_spheres[i].check_intersection(vector_through_pixel, base_point_of_vector) == 1){
            
            intersec_point = cur_space.array_of_spheres[i].get_intersection_point(vector_through_pixel, base_point_of_vector);

            if ( distance(intersec_point, base_point_of_vector) > distance_to_prev_object){
                
                for (int j = 0; j < number_of_light_sources; j++){

                    tmp_color = cur_space.array_of_spheres[i].illumination(vector_through_pixel, arr_of_light_sources[j], intersec_point);

                    pixel_color.R += tmp_color.R;
                    pixel_color.G += tmp_color.G;
                    pixel_color.B += tmp_color.B;
                }                
            }
        }
    }

    pixel_color = normolize(pixel_color);
    return pixel_color;
}

void Rendering_engine::show_window(){

    colour pixel_color;
    Matrix vector_through_pixel(3, 1, (float)0);

    for (int i = 0; i < vertical_window_size; i++){

        for (int j = 0; j < horizontal_window_size; j++){ 

            vector_through_pixel = screen_axis_x * (j - horizontal_window_size / 2) + screen_axis_y * (i - vertical_window_size / 2) - (distance_to_screen) * screen_normal;
    
            pixel_color = this->get_pixel_color(vector_through_pixel, observ_position);

            this->set_pixel(j, i, pixel_color);
        }
    }
    printf("frame\n");
    window.display();
}