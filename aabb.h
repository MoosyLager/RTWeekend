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

    AABB(const AABB &box0, const AABB &box1)
    {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval &Axis(int n) const
    {
        if ( n == 1 ) return y;
        if ( n == 2 ) return z;
        return x;
    }

    int MaxAxes() const
    {
        double xSize = x.Size();
        double ySize = y.Size();
        double zSize = z.Size();

        if ( xSize >= ySize && xSize >= zSize ) return 0;
        if ( ySize >= xSize && ySize >= zSize ) return 1;
        return 2;
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

    AABB Pad()
    {
        // Return an AABB that has no side narrower than some delta, padding if necessary
        double delta = 0.0001;
        Interval newX = (x.Size() >= delta) ? x : x.Expand(delta);
        Interval newY = (y.Size() >= delta) ? y : y.Expand(delta);
        Interval newZ = (z.Size() >= delta) ? z : z.Expand(delta);

        return AABB(newX, newY, newZ);
    }
};

AABB operator+(const AABB &boundingBox, const Vec3 &offset)
{
    return AABB(boundingBox.x + offset.X(), boundingBox.y + offset.Y(), boundingBox.z + offset.Z());
}

AABB operator+(const Vec3 &offset, const AABB &boundingBox)
{
    return boundingBox + offset;
}

AABB SurroundingBox(const AABB &box0, const AABB &box1)
{
    Interval x(fmin(box0.x.min, box1.x.min), fmax(box0.x.max, box1.x.max));
    Interval y(fmin(box0.y.min, box1.y.min), fmax(box0.y.max, box1.y.max));
    Interval z(fmin(box0.z.min, box1.z.min), fmax(box0.z.max, box1.z.max));
    return AABB(x, y, z);
}

AABB SurroundingBox(const AABB &box, const Point3 &p)
{
}

#endif