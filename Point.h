#ifndef POINT_H_
#define POINT_H_

class Point {
public:
    int x;
    int y;

    Point();

    Point(int x, int y);

    float distance(Point other);
};


#endif