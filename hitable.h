#ifndef HITABLE_H
#define HITABLE_H

#include "aabb.h"
#include "rtweekend.h"

class Material;

class HitRecord
{
public:
    Point3 point;
    Vec3 normal;
    shared_ptr<Material> material;
    double t;
    double u;
    double v;
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

    virtual AABB BoundingBox() const = 0;
};

class Translate : public Hitable
{
private:
    shared_ptr<Hitable> object;
    Vec3 offset;
    AABB boundingBox;

public:
    Translate(shared_ptr<Hitable> p, const Vec3 &displacement)
        : object(p), offset(displacement)
    {
        boundingBox = object->BoundingBox() + offset;
    }

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        // Move ray backwards by the offset
        Ray offsetRay(ray.Origin() - offset, ray.Direction(), ray.Time());

        // Determine where (if any) an intersection occurs along the offset ray
        if ( !object->Hit(offsetRay, rayT, record) ) return false;

        // Move the intersection point forwards by the offset
        record.point += offset;

        return true;
    }

    AABB BoundingBox() const override { return boundingBox; }
};

#endif