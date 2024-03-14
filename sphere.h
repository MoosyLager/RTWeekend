#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "vec3.h"

class Sphere : public Hitable
{
private:
    Point3 centre;
    float radius;

public:
    Sphere(Point3 _centre, float _radius) : centre(_centre), radius(_radius) {}

    bool Hit(const Ray &ray, float rayTMin, float rayTMax, HitRecord &record) const override
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
        if ( root <= rayTMin || rayTMax <= root ) {
            root = (-halfB + sqrtD) / a;
            if ( root <= rayTMin || rayTMax <= root ) {
                return false;
            }
        }

        record.t = root;
        record.p = ray.At(record.t);
        record.normal = (record.p - centre) / radius;

        return true;
    }
};

#endif