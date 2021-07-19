#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <math.h>

class Flatness;
class Matrix;

class Point{

public:

    float x;
    float y;
    float z;

    Point(){

        x = 0;
        y = 0;
        z = 0;
    }

    Point(float x_data, float y_data, float z_data){

        x = x_data;
        y = y_data;
        z = z_data;
    }

    Point(const Point& old_point){

        x = old_point.x;
        y = old_point.y;
        z = old_point.z;
    }

    ~Point(){
        
    }

    Point& operator = (const Point& old_point){

        x = old_point.x;
        y = old_point.y;
        z = old_point.z;

        return *this;
    }

    void print(FILE* output_file){

        fprintf(output_file, "(%f, %f, %f)", x, y, z);
    }

    Point& operator = (Point&& old_point){

        x = old_point.x;
        y = old_point.y;
        z = old_point.z;

        return *this;
    }

    friend int operator == (const Point& L_point, const Point& R_point);

    friend int operator != (const Point& L_point, const Point& R_point);

    friend float distance(Point& fir_point, Point& sec_point);

    friend Point get_point(FILE* input_file);

    friend Matrix create_vector(const Point& fir_point, const Point& sec_point);

    friend float get_koef_D(Point& point, Flatness& flat);
    
    friend class Sphere;

    friend class Flatness;
};

#endif