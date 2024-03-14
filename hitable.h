#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"

class HitRecord
{
public:
    Point3 p;
    Vec3 normal;
    float t;
};

class Hitable
{
public:
    virtual ~Hitable() = default;

    virtual bool Hit(const Ray &ray, float rayTMin, float rayTMax, HitRecord &record) const = 0;
};

#endif