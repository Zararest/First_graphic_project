#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <math.h>
#include "Matrix.h"
#include "Point.h"

class Sphere;
class Flatness;

void go_to_end_of_line(FILE* file);

char fgetc_without_space(FILE* input_file);

Matrix get_vector(FILE* input_file);

Point get_point(FILE* input_file);

Matrix create_vector(const Point& fir_point, const Point& sec_point);

float get_koef_D(Point& point, Flatness& flat);

float projection(Matrix& vector, Matrix&  axis);

float distance(Point& fir_point, Point& sec_point);

float distance(Flatness& flat, Point& point);

float distance(Point& fir_point, Point& sec_point);

int check_intersection_with_bound_points(Flatness& flat, Matrix& vector, Point& base_point_of_vector);

int check_intersection_with_sphere(Sphere& sphere, Matrix& vector, Point& base_point_of_vector);

//Point get_intersection_point(Sphere& space, Matrix& vector, Point& base_point_of_vector);


typedef struct color_struct {
    
    int R = 255;
    int G = 255;
    int B = 255;
} colour;


//-----------------------------------------------------------
class Field{

public:

    virtual Matrix return_force(Sphere& cur_obj); //возвращает силу
};
//-----------------------------------------------------------

//-----------------------------------------------------------
class Light_source{

private:

    Point position;

    float brightness;

public:

    Light_source(FILE* input_file);

    Light_source(const Light_source&) = delete;

    Light_source(Light_source&&) = delete;

    ~Light_source();

    Light_source& operator = (const Light_source&) = delete;

    Light_source& operator = (Light_source&& rv);

    void get_info(FILE* output_file);

    friend class Sphere;
};
//-----------------------------------------------------------

//-----------------------------------------------------------
class Object{

protected:

    float transpendecy;   //прозрачность

    float surface;      //параметр рассеивания при отражении

    int* color;

    int number_of_param; //количество физ параметров

    float* phys_param;    //набор физических параметров для полей 

public:

    Object();

    ~Object();

    Object(const Object&) = delete;

    Object(Object&&) = delete;

    //virtual int check_intersection(Matrix& vector, Point& base_point_of_vector);

    //virtual Point get_intersection_point(Matrix& vector, Point& base_point_of_vector);

    //virtual colour illumination(Matrix& vector, Light_source& cur_light_source, Point& intersec_point); //возвращает угол между вектором взгляда и вектором отражения для определенного исочника света

    friend void get_object_params(FILE* input_file, Object& tmp_obj);
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Sphere: public Object{

private: 

    Point centre;

    float radius;

    float mass;         //масса для столкновений(-1 значит неподвижный 0 - не учавствует в столкновениях)

    Matrix velocity;

    float dissipation;  //характеристика упругости столкновения

    //Matrix axis;        //для вращения(потом можно добавить)

public:

    Sphere(FILE* input_file);

    Sphere(const Sphere& old_obj) = delete;

    Sphere(Sphere&& old_obj);

    ~Sphere();

    void shift(float delta_t);
    
    Sphere& operator = (Sphere&& rv);

    Sphere& operator = (const Sphere&) = delete;

    void change_position(float delta_x, float delta_y, float delta_z);

    int check_intersection(Matrix& vector, Point& base_point_of_vector);

    Point get_intersection_point(Matrix& vector, Point& base_point_of_vector);

    colour illumination(Matrix& vector, Light_source& cur_light_source, Point& intersec_point); 
    
    friend int operator == (const Sphere& fir_sph, const Sphere& sec_sph);

    void get_info(FILE* output_file);

    void init();

    friend float get_time_before_bump(Sphere& fir_sphere, Sphere& sec_sphere);

    friend float get_time_before_bump(Flatness& flat, Sphere& sphere);

    friend void calculate_bump(Sphere& fir_sphere, Sphere& sec_sphere);

    friend void calculate_bump(Flatness& flat, Sphere& sphere);

    friend void shift_sphere(Sphere& cur_sph);

    friend class Elastic_interaction;

    friend class Physics_engine;
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Flatness: public Object{

private:

    Matrix normal;

    Point base_point;         //точка, принадлежащая плоскости

    Point* bounding_points;  //набор точек на плоскости, которые ограничивают плоскость(первая и последняя точки одинаковые)

    Matrix velocity;

public:

    Flatness(FILE* input_file);

    Flatness(const Flatness& old_obj) = delete;

    Flatness(Flatness&& old_obj);

    ~Flatness();

    void shift(float delta_t);

    Flatness& operator = (Flatness&& rv);

    Flatness& operator = (const Flatness&) = delete;

    void change_position(float delta_x, float delta_y, float delta_z); //можно добавить вращение у плскости

    int check_intersection(Matrix& vector, Point& base_point_of_vector);

    Point get_intersection_point(Matrix& vector, Point& base_point_of_vector);

    void get_info(FILE* output_file);

    void init();

    colour illumination(Matrix& vector, Light_source& cur_light_source, Point& intersec_point);

    friend float get_time_before_bump(Flatness& flat, Sphere& sphere);

    friend void calculate_bump(Flatness& flat, Sphere& sphere);

    friend void shift_flat(Flatness& cur_flat);

    friend float distance(Flatness& flat, Point& point);

    friend float get_koef_D(Point& point, Flatness& flat);
};
//-----------------------------------------------------------

#endif
