#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "vec3.h"

class Sphere : public Hitable
{
private:
    Point3 centre1;
    double radius;
    shared_ptr<Material> material;
    bool isMoving;
    Vec3 centreVec;
    AABB boundingBox;

    Point3 Centre(double time) const
    {
        // Linearly interpolate from centre1 to centre2 according to time, where t=0 yields
        // centre1 and t=1 yields centre2
        return centre1 + time * centreVec;
    }

public:
    // Stationary Sphere
    Sphere(Point3 _centre, double _radius, shared_ptr<Material> _material)
        : centre1(_centre), radius(_radius), material(_material), isMoving(false)
    {
        auto radiusVec = Vec3(radius, radius, radius);
    }

    // Moving Sphere
    Sphere(Point3 _centre1, Point3 _centre2, double _radius, shared_ptr<Material> _material)
        : centre1(_centre1), radius(_radius), material(_material), isMoving(true)
    {
        auto radiusVec = Vec3(radius, radius, radius);
        AABB box1(_centre1 - radiusVec, _centre1 + radiusVec);
        AABB box2(_centre2 - radiusVec, _centre2 + radiusVec);
        boundingBox = AABB(box1, box2);

        centreVec = _centre2 - _centre1;
    }

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        Point3 centre = isMoving ? Centre(ray.Time()) : centre1;
        Vec3 oc = ray.Origin() - centre;
        auto a = ray.Direction().LengthSquared();
        auto halfB = Dot(oc, ray.Direction());
        auto c = oc.LengthSquared() - radius * radius;

        auto discriminant = halfB * halfB - a * c;
        if ( discriminant < 0 ) return false;
        auto sqrtD = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range
        auto root = (-halfB - sqrtD) / a;
        if ( !rayT.Surrounds(root) ) {
            root = (-halfB + sqrtD) / a;
            if ( !rayT.Surrounds(root) ) {
                return false;
            }
        }

        record.t = root;
        record.point = ray.At(record.t);
        Vec3 outwardNormal = (record.point - centre1) / radius;
        record.SetFaceNormal(ray, outwardNormal);
        record.material = material;

        return true;
    }

    AABB BoundingBox() const override { return boundingBox; }
};

#endif