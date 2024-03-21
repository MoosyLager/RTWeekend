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
    Vec3 normal;
    double D;

public:
    Quad(const Point3 &_Q, const Vec3 &_u, const Vec3 &_v, shared_ptr<Material> _material)
        : Q(_Q), u(_u), v(_v), material(_material)
    {
        auto n = Cross(u, v);
        normal = UnitVector(n);
        D = Dot(normal, Q);

        SetBoundingBox();
    }

    virtual void SetBoundingBox()
    {
        boundingBox = AABB(Q, Q + u + v).Pad();
    }

    AABB BoundingBox() const override { return boundingBox; }

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        auto denominator = Dot(normal, ray.Direction());

        // No hit if the ray is parallel to the plane
        if ( fabs(denominator) < 1e-8 ) return false;

        // Return false if the hit point parameter t is outside the ray interval
        auto t = (D - Dot(normal, ray.Origin())) / denominator;
        if ( !rayT.Contains(t) ) return false;

        auto intersection = ray.At(t);

        record.t = t;
        record.point = intersection;
        record.material = material;
        record.SetFaceNormal(ray, normal);

        return true;
    }
};

#endif