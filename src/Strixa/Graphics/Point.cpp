#include <Strixa/Graphics/Point.h>

using Strixa::Graphics::Point;


Point::Point(long x,long y,long z)
: x(raw[0]),y(raw[1]),z(raw[2])
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point(const Point& copy)
: x(raw[0]),y(raw[1]),z(raw[2])
{
    *this = copy;
}

bool Point::operator==(const Point& comp) const
{
    return this->x == comp.x && this->y == comp.y && this->z == comp.z;
}

bool Point::operator!=(const Point& comp) const
{
    return !(*this == comp);
}

Point& Point::operator=(const Point& copy)
{
    this->x = copy.x;
    this->y = copy.y;
    this->z = copy.z;

    return *this;
}