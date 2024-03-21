#ifndef QUAD_H
#define QUAD_H

#include <cmath>

#include "hitable.h"
#include "rtweekend.h"

class Quad : public Hitable
{
private:
    Point3 Q;
    Vec3 u, v;
    shared_ptr<Material> material;
    AABB boundingBox;
    Vec3 normal;
    double D;
    Vec3 w;

public:
    Quad(const Point3 &_Q, const Vec3 &_u, const Vec3 &_v, shared_ptr<Material> _material)
        : Q(_Q), u(_u), v(_v), material(_material)
    {
        auto n = Cross(u, v);
        normal = UnitVector(n);
        D = Dot(normal, Q);
        w = n / Dot(n, n);

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

        // Determine the hit point lies within the planar shape using its plane coordinates
        auto intersection = ray.At(t);
        Vec3 planarHitpointVector = intersection - Q;
        auto alpha = Dot(w, Cross(planarHitpointVector, v));
        auto beta = Dot(w, Cross(u, planarHitpointVector));

        if ( !isInterior(alpha, beta, record) ) return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true
        record.t = t;
        record.point = intersection;
        record.material = material;
        record.SetFaceNormal(ray, normal);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord &record) const
    {
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true

        if ( (a < 0) || (1 < a) || (b < 0) || (1 < b) ) return false;

        record.u = a;
        record.v = b;
        return true;
    }
};

#endif