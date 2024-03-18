#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

class HitRecord;

class Material
{
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
private:
    Colour albedo;

public:
    Lambertian(const Colour &a) : albedo(a) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        auto scatterDirection = record.normal + RandomUnitVector();

        // Catch degenerate scatter direction
        if ( scatterDirection.NearZero() ) scatterDirection = record.normal;

        scattered = Ray(record.point, scatterDirection);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material
{
private:
    Colour albedo;

public:
    Metal(const Colour &a) : albedo(a) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = Reflect(rayIn.Direction(), record.normal);
        scattered = Ray(record.point, reflected);
        attenuation = albedo;
        return true;
    }
};

#endif