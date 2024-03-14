#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
private:
    Point3 origin;
    Vec3 direction;

public:
    Ray() {}

    Ray(const Point3 &orig, const Vec3 &dir) : origin(orig), direction(dir) {}

    Point3 Origin() const { return origin; }

    Point3 Direction() const { return direction; }

    Point3 At(float t) const
    {
        return origin + t * direction;
    }
};

#endif