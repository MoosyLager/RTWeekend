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

    virtual double PDFValue(const Point3 &origin, const Vec3 &direction) const = 0;

    virtual Vec3 Random(const Point3 &origin) const = 0;
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

    double PDFValue(const Point3 &origin, const Vec3 &direction) const override
    {
        return 0.0;
    }

    Vec3 Random(const Point3 &origin) const override
    {
        return Vec3(1, 0, 0);
    }
};

class RotateY : public Hitable
{
private:
    shared_ptr<Hitable> object;
    double sinTheta;
    double cosTheta;
    AABB boundingBox;

public:
    RotateY(shared_ptr<Hitable> p, double angle) : object(p)
    {
        auto radians = DegreesTooRadians(angle);
        sinTheta = sin(radians);
        cosTheta = cos(radians);
        boundingBox = object->BoundingBox();

        Point3 min(maxDouble, maxDouble, maxDouble);
        Point3 max(-maxDouble, -maxDouble, -maxDouble);

        for ( int i = 0; i < 2; i++ ) {
            for ( int j = 0; j < 2; j++ ) {
                for ( int k = 0; k < 2; k++ ) {
                    auto x = i * boundingBox.x.max + (1 - i) * boundingBox.x.min;
                    auto y = j * boundingBox.y.max + (1 - j) * boundingBox.y.min;
                    auto z = k * boundingBox.z.max + (1 - k) * boundingBox.z.min;

                    auto newX = cosTheta * x + sinTheta * z;
                    auto newZ = -sinTheta * x + cosTheta * z;

                    Vec3 tester(newX, y, newZ);

                    for ( int c = 0; c < 3; c++ ) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        boundingBox = AABB(min, max);
    }

    bool Hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override
    {
        // Change the ray from world space to object space
        auto origin = ray.Origin();
        auto direction = ray.Direction();

        origin[0] = cosTheta * ray.Origin()[0] - sinTheta * ray.Origin()[2];
        origin[2] = sinTheta * ray.Origin()[0] + cosTheta * ray.Origin()[2];

        direction[0] = cosTheta * ray.Direction()[0] - sinTheta * ray.Direction()[2];
        direction[2] = sinTheta * ray.Direction()[0] + cosTheta * ray.Direction()[2];

        Ray rotatedRay(origin, direction, ray.Time());

        // Determine where (if any) an intersection occurs in object space
        if ( !object->Hit(rotatedRay, ray_t, rec) )
            return false;

        // Change the intersection point from object space to world space
        auto point = rec.point;
        point[0] = cosTheta * rec.point[0] + sinTheta * rec.point[2];
        point[2] = -sinTheta * rec.point[0] + cosTheta * rec.point[2];

        // Change the normal from object space to world space
        auto normal = rec.normal;
        normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
        normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

        rec.point = point;
        rec.normal = normal;

        return true;
    }

    AABB BoundingBox() const override { return boundingBox; }

    double PDFValue(const Point3 &origin, const Vec3 &direction) const override
    {
        return 0.0;
    }

    Vec3 Random(const Point3 &origin) const override
    {
        return Vec3(1, 0, 0);
    }
};

#endif