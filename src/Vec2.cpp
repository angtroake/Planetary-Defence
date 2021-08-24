#include "Vec2.h"
#include <math.h>

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float& val) const
{
    return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float& val) const
{
    return Vec2(x * val, y * val);
}

bool Vec2::operator == (const Vec2& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return !(*this == rhs);
}

float Vec2::operator * (const Vec2& rhs) const
{
    return x * rhs.y - rhs.x * y;
}

float Vec2::dist(const Vec2& rhs) const
{
    return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
}

float Vec2::mag() const
{
    return sqrt(x * x + y * y);
}
