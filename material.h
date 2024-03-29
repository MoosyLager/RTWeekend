#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

#include "texture.h"

class HitRecord;

class Material
{
public:
    virtual ~Material() = default;

    virtual Colour Emitted(double u, double v, const Point3 &p) const
    {
        return Colour(0, 0, 0);
    }

    virtual bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
private:
    shared_ptr<Texture> albedo;

public:
    Lambertian(const Colour &a) : albedo(make_shared<SolidColour>(a)) {}

    Lambertian(shared_ptr<Texture> a) : albedo(a) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        auto scatterDirection = record.normal + RandomUnitVector();

        // Catch degenerate scatter direction
        if ( scatterDirection.NearZero() ) scatterDirection = record.normal;

        scattered = Ray(record.point, scatterDirection, rayIn.Time());
        attenuation = albedo->Value(record.u, record.v, record.point);
        return true;
    }
};

class Metal : public Material
{
private:
    Colour albedo;
    double fuzz;

public:
    Metal(const Colour &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = Reflect(rayIn.Direction(), record.normal);
        scattered = Ray(record.point, reflected + fuzz * RandomInUnitSphere(), rayIn.Time());
        attenuation = albedo;
        return (Dot(scattered.Direction(), record.normal) > 0);
    }
};

class Dielectric : public Material
{
private:
    double refractiveIndex;

    static double Reflectance(double cosine, double refIdx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refIdx) / (1 + refIdx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

public:
    Dielectric(double ir) : refractiveIndex(ir) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        attenuation = Colour(1.0, 1.0, 1.0);
        double refractionRatio = record.frontFace ? (1.0 / refractiveIndex) : refractiveIndex;

        Vec3 unitDirection = UnitVector(rayIn.Direction());
        double cosTheta = fmin(Dot(-unitDirection, record.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        Vec3 direction;

        if ( cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble() ) {
            direction = Reflect(unitDirection, record.normal);
        } else {
            direction = Refract(unitDirection, record.normal, refractionRatio);
        }

        scattered = Ray(record.point, direction, rayIn.Time());
        return true;
    }
};

class DiffuseLight : public Material
{
private:
    shared_ptr<Texture> emit;

public:
    DiffuseLight(shared_ptr<Texture> a) : emit(a) {}

    DiffuseLight(Colour c) : emit(make_shared<SolidColour>(c)) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        return false;
    }

    Colour Emitted(double u, double v, const Point3 &p) const override
    {
        return emit->Value(u, v, p);
    }
};

class Isotropic : public Material
{
private:
    shared_ptr<Texture> albedo;

public:
    Isotropic(Colour c) : albedo(make_shared<SolidColour>(c)) {}

    Isotropic(shared_ptr<Texture> a) : albedo(a) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, Colour &attenuation, Ray &scattered) const override
    {
        scattered = Ray(record.point, RandomUnitVector(), rayIn.Time());
        attenuation = albedo->Value(record.u, record.v, record.point);
        return true;
    }
};

#endif