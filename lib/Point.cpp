#include "headers/Point.h"

int operator == (const Point& L_point, const Point& R_point){

    if ((L_point.x == R_point.x) && (L_point.y == R_point.y) && (L_point.z == R_point.z)){

        return 1;
    } else{

        return 0;
    }
}

int operator != (const Point& L_point, const Point& R_point){

    return !(L_point == R_point);
}