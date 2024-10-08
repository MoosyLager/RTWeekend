#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include "onb.h"
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

    static void GetSphereUV(const Point3 &p, double &u, double &v)
    {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.Y());
        auto phi = atan2(-p.Z(), p.X()) + PI;

        u = phi / (2 * PI);
        v = theta / PI;
    }

    static Vec3 RandomToSphere(double radius, double distanceSquared)
    {
        auto r1 = RandomDouble();
        auto r2 = RandomDouble();
        auto z = 1 + r2 * (std::sqrt(1 - radius * radius / distanceSquared) - 1);

        auto phi = 2 * PI * r1;
        auto x = std::cos(phi) * std::sqrt(1 - z * z);
        auto y = std::sin(phi) * std::sqrt(1 - z * z);

        return Vec3(x, y, z);
    }

public:
    // Stationary Sphere
    Sphere(Point3 _centre, double _radius, shared_ptr<Material> _material)
        : centre1(_centre), radius(_radius), material(_material), isMoving(false)
    {
        auto radiusVec = Vec3(radius, radius, radius);
        boundingBox = AABB(centre1 - radiusVec, centre1 + radiusVec);
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
        GetSphereUV(outwardNormal, record.u, record.v);
        record.material = material;

        return true;
    }

    AABB BoundingBox() const override { return boundingBox; }

    double PDFValue(const Point3 &origin, const Vec3 &direction) const override
    {
        // This method only works for stationary spheres.

        HitRecord record;
        if ( !this->Hit(Ray(origin, direction, 0.0), Interval(0.001, maxDouble), record) ) return 0;

        auto cosThetaMax = std::sqrt(1 - radius * radius / (centre1 - origin).LengthSquared());
        auto solidAngle = 2 * PI * (1 - cosThetaMax);

        return 1 / solidAngle;
    }

    Vec3 Random(const Point3 &origin) const override
    {
        Vec3 direction = centre1 - origin;
        auto distanceSquared = direction.LengthSquared();
        ONB uvw(direction);
        return uvw.Transform(RandomToSphere(radius, distanceSquared));
    }
};

#endif