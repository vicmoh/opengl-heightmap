#include "point.h"

Point* new_Point(long double x, long double y, long double z){
    Point* this = malloc(sizeof(Point));
    this->x = x;
    this->y = y;
    this->z = z;
    return this;
}

