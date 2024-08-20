#ifndef QUAD_H
#define QUAD_H

#include <cmath>

#include "hitable.h"
#include "hitableList.h"
#include "rtweekend.h"

class Quad : public Hitable
{
private:
    Point3 Q;
    Vec3 u, v;
    Vec3 w;
    shared_ptr<Material> material;
    AABB boundingBox;
    Vec3 normal;
    double D;
    double area;

public:
    Quad(const Point3 &_Q, const Vec3 &_u, const Vec3 &_v, shared_ptr<Material> _material)
        : Q(_Q), u(_u), v(_v), material(_material)
    {
        auto n = Cross(u, v);
        normal = UnitVector(n);
        D = Dot(normal, Q);
        w = n / Dot(n, n);

        area = n.Length();

        SetBoundingBox();
    }

    void SetBoundingBox()
    {
        // Compute the bounding box of all four vertices
        auto boundingBoxDiagonal1 = AABB(Q, Q + u + v);
        auto boundingBoxDiagonal2 = AABB(Q + u, Q + v);
        boundingBox = AABB(boundingBoxDiagonal1, boundingBoxDiagonal2);
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

    double PDFValue(const Point3 &origin, const Vec3 &direction) const override
    {
        HitRecord record;
        if ( !this->Hit(Ray(origin, direction, 0.0), Interval(0.001, maxDouble), record) ) return 0;

        auto distanceSquared = record.t * record.t * direction.LengthSquared();
        auto cosine = std::fabs(Dot(direction, record.normal) / direction.Length());

        return distanceSquared / (cosine * area);
    }

    Vec3 Random(const Point3 &origin) const override
    {
        auto p = Q + (RandomDouble() * u) + (RandomDouble() * v);
        return p - origin;
    }
};

// class Box : public Hitable
// {
// public:
//     shared_ptr<HitableList> sides;

//     Box(const Point3 &a, const Point3 &b, shared_ptr<Material> material)
//     {
//         // Construct the two opposite verticles with the minimum and maximum coordinates.
//         auto min = Point3(fmin(a.X(), b.X()), fmin(a.Y(), b.Y()), fmin(a.Z(), b.Z()));
//         auto max = Point3(fmax(a.X(), b.X()), fmax(a.Y(), b.Y()), fmax(a.Z(), b.Z()));

//         auto dx = Vec3(max.X() - min.X(), 0, 0);
//         auto dy = Vec3(0, max.Y() - min.Y(), 0);
//         auto dz = Vec3(0, 0, max.Z() - min.Z());

//         sides->Add(make_shared<Quad>(Point3(min.X(), min.Y(), max.Z()), dx, dy, material));  // front
//         sides->Add(make_shared<Quad>(Point3(max.X(), min.Y(), max.Z()), -dz, dy, material)); // right
//         sides->Add(make_shared<Quad>(Point3(max.X(), min.Y(), min.Z()), -dx, dy, material)); // back
//         sides->Add(make_shared<Quad>(Point3(min.X(), min.Y(), min.Z()), dz, dy, material));  // left
//         sides->Add(make_shared<Quad>(Point3(min.X(), max.Y(), max.Z()), dx, -dz, material)); // top
//         sides->Add(make_shared<Quad>(Point3(min.X(), min.Y(), min.Z()), dx, dz, material));  // bottom
//     }

//     shared_ptr<HitableList> Sides() const { return sides; }
// };

inline shared_ptr<HitableList> Box(const Point3 &a, const Point3 &b, shared_ptr<Material> material)
{
    // Returns the 3D box (six sides) that contains the top opposite vertices a & b.

    auto sides = make_shared<HitableList>();

    // Construct the two opposite verticles with the minimum and maximum coordinates.
    auto min = Point3(fmin(a.X(), b.X()), fmin(a.Y(), b.Y()), fmin(a.Z(), b.Z()));
    auto max = Point3(fmax(a.X(), b.X()), fmax(a.Y(), b.Y()), fmax(a.Z(), b.Z()));

    auto dx = Vec3(max.X() - min.X(), 0, 0);
    auto dy = Vec3(0, max.Y() - min.Y(), 0);
    auto dz = Vec3(0, 0, max.Z() - min.Z());

    sides->Add(make_shared<Quad>(Point3(min.X(), min.Y(), max.Z()), dx, dy, material));  // front
    sides->Add(make_shared<Quad>(Point3(max.X(), min.Y(), max.Z()), -dz, dy, material)); // right
    sides->Add(make_shared<Quad>(Point3(max.X(), min.Y(), min.Z()), -dx, dy, material)); // back
    sides->Add(make_shared<Quad>(Point3(min.X(), min.Y(), min.Z()), dz, dy, material));  // left
    sides->Add(make_shared<Quad>(Point3(min.X(), max.Y(), max.Z()), dx, -dz, material)); // top
    sides->Add(make_shared<Quad>(Point3(min.X(), min.Y(), min.Z()), dx, dz, material));  // bottom

    return sides;
}

#endif