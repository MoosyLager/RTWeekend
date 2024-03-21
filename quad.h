#ifndef QUAD_H
#define QUAD_H

#include "rtweekend.h"

#include "hitable.h"

class Quad : public Hitable
{
private:
    Point3 Q;
    Vec3 u, v;
    shared_ptr<Material> material;
    AABB boundingBox;

public:
    Quad(const Point3 &_Q, const Vec3 &_u, const Vec3 &_v, shared_ptr<Material> _material)
        : Q(_Q), u(_u), v(_v), material(_material)
    {
        SetBoundingBox();
    }

    virtual void SetBoundingBox()
    {
        boundingBox = AABB(Q, Q + u + v).Pad();
    }

    AABB BoundingBox() const override { return boundingBox; }

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        return false; // To be implemented
    }
};

#endif