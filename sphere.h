#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "vec3.h"

class Sphere : public Hitable
{
private:
    Point3 centre;
    double radius;

public:
    Sphere(Point3 _centre, double _radius) : centre(_centre), radius(_radius) {}

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
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
        record.p = ray.At(record.t);
        Vec3 outwardNormal = (record.p - centre) / radius;
        record.SetFaceNormal(ray, outwardNormal);

        return true;
    }
};

#endif