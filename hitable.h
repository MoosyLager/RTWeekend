#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"

class HitRecord
{
public:
    Point3 p;
    Vec3 normal;
    double t;
    bool frontFace;

    void SetFaceNormal(const Ray &ray, const Vec3 &outwardNormal)
    {
        // Sets the hit record normal normal vector
        // NOTES: the parameter `outward normal` is assumed to have unit length

        frontFace = Dot(ray.Direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hitable
{
public:
    virtual ~Hitable() = default;

    virtual bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const = 0;
};

#endif