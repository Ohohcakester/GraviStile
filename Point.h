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

class Vec2 {
public:
    float x;
    float y;

    Vec2();

    Vec2(float x, float y);

    Vec2& operator+=(const Vec2 &o);
    Vec2& operator-=(const Vec2 &o);
    bool operator==(const Vec2 &o) const;
    bool operator!=(const Vec2 &o) const;

};

Vec2 operator+(const Vec2& o, const Vec2& o2);
Vec2 operator-(const Vec2& o, const Vec2& o2);


#endif