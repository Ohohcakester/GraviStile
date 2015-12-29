#include "Point.h"
#include <math.h>

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

float Point::distance(Point other) {
    return sqrt(pow(x - other.x, 2.0) + pow(y - other.y, 2.0));
}
