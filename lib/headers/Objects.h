#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <math.h>
#include "Matrix.h"
#include "Point.h"


void go_to_end_of_line(FILE* file);

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

    //virtual float illumination(Matrix vector, int light_number); //возвращает угол между вектором взгляда и вектором отражения для определенного исочника света

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
    
    Sphere& operator = (Sphere&& rv);

    Sphere& operator = (const Sphere&) = delete;

    void change_position(float delta_x, float delta_y, float delta_z);

    //float illumination(const Matrix& ray_light, int light_number);

    void get_info(FILE*);
};
//-----------------------------------------------------------


//-----------------------------------------------------------
class Flatness: public Object{

private:

    Matrix normal;

    Point base_point;         //точка, принадлежащая плоскости

    Point* bounding_points;  //набор точек на плоскости, которые ограничивают плоскость(первая и последняя точки одинаковые)

    Matrix velocity;

    //float* phys_param;    //набор физических параметров

public:

    Flatness(FILE* input_file);

    Flatness(const Flatness& old_obj) = delete;

    Flatness(Flatness&& old_obj);

    ~Flatness();

    Flatness& operator = (Flatness&& rv);

    Flatness& operator = (const Flatness&) = delete;

    void change_position(float delta_x, float delta_y, float delta_z); //можно добавить вращение у плскости

    void get_info(FILE* output_file);

    //float illumination(const Matrix& ray_light, int light_number); //данные об источники света берутся из сатитик переменной отрисовки 
};
//-----------------------------------------------------------

#endif
