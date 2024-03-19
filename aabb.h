#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class AABB
{
public:
    Interval x, y, z;

    AABB() {} // The default AABB is empty, since Intervals are empty by default

    AABB(const Interval &ix, const Interval &iy, const Interval &iz)
        : x(ix), y(iy), z(iz) {}

    AABB(const Point3 &a, const Point3 &b)
    {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.
        x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
        y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
        z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
    }

    const Interval &Axis(int n) const
    {
        if ( n == 1 ) return y;
        if ( n == 2 ) return z;
        return x;
    }

    bool Hit(const Ray &ray, Interval rayT) const
    {
        for ( int a = 0; a < 3; a++ ) {
            auto inverseDirection = 1 / ray.Direction()[a];
            auto origin = ray.Origin()[a];

            auto t0 = (Axis(a).min - origin) * inverseDirection;
            auto t1 = (Axis(a).max - origin) * inverseDirection;

            if ( inverseDirection < 0 ) std::swap(t0, t1);

            if ( t0 > rayT.min ) rayT.min = t0;
            if ( t1 < rayT.max ) rayT.max = t1;

            if ( rayT.max <= rayT.min ) return false;
        }
        return true;
    }
};

#endif