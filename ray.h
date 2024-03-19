#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
private:
    Point3 origin;
    Vec3 direction;
    double time;

public:
    Ray() {}

    Ray(const Point3 &orig, const Vec3 &dir) : origin(orig), direction(dir), time(0) {}

    Ray(const Point3 &orig, const Vec3 &dir, double tm = 0.0)
        : origin(orig), direction(dir), time(tm) {}

    Point3 Origin() const { return origin; }

    Point3 Direction() const { return direction; }

    double Time() const { return time; }

    Point3 At(double t) const
    {
        return origin + t * direction;
    }
};

#endif