#include "Point.h"
#include <math.h>

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

float Point::distance(Point other) {
    return sqrt(pow(x - other.x, 2.0) + pow(y - other.y, 2.0));
}

Vec2::Vec2(float x, float y) : x(x), y(y)  {
}

Vec2 operator+(const Vec2& o, const Vec2& o2) {
    return Vec2(o.x + o2.x, o.y + o2.y);
}

Vec2 operator-(const Vec2& o, const Vec2& o2) {
    return Vec2(o.x - o2.x, o.y - o2.y);
}

Vec2& Vec2::operator+=(const Vec2& o) {
    x += o.x;
    y += o.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& o) {
    x -= o.x;
    y -= o.y;
    return *this;
}

bool Vec2::operator==(const Vec2 &o) const {
    // simplest form is guaranteed unique.
    return (x == o.x) && (y == o.y);
}

bool Vec2::operator!=(const Vec2 &o) const {
    return !(*this == o);
}
