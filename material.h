#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

#include "pdf.h"
#include "texture.h"

class HitRecord;

class ScatterRecord
{
public:
    Colour attenuation;
    shared_ptr<PDF> pdfPtr;
    bool skipPdf;
    Ray skipPdfRay;
};
class Material
{
public:
    virtual ~Material() = default;

    virtual Colour Emitted(const Ray &rayIn, const HitRecord &record, double u, double v, const Point3 &p) const
    {
        return Colour(0, 0, 0);
    }

    virtual bool Scatter(const Ray &rayIn, const HitRecord &record, ScatterRecord &sRecord) const = 0;

    virtual double ScatteringPDF(const Ray &rayIn, const HitRecord &record, const Ray &scattered) const { return 0; }
};

class Lambertian : public Material
{
private:
    shared_ptr<Texture> albedo;

public:
    Lambertian(const Colour &a) : albedo(make_shared<SolidColour>(a)) {}

    Lambertian(shared_ptr<Texture> a) : albedo(a) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, ScatterRecord &sRecord) const override
    {
        sRecord.attenuation = albedo->Value(record.u, record.v, record.point);
        sRecord.pdfPtr = make_shared<CosinePDF>(record.normal);
        sRecord.skipPdf = false;
        return true;
    }

    double ScatteringPDF(const Ray &rayIn, const HitRecord &record, const Ray &scattered) const override
    {
        auto cosTheta = Dot(record.normal, UnitVector(scattered.Direction()));
        return cosTheta < 0 ? 0 : cosTheta / PI;
    }
};

class Metal : public Material
{
private:
    Colour albedo;
    double fuzz;

public:
    Metal(const Colour &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, ScatterRecord &sRecord) const override
    {
        Vec3 reflected = Reflect(rayIn.Direction(), record.normal);
        reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());

        sRecord.attenuation = albedo;
        sRecord.pdfPtr = nullptr;
        sRecord.skipPdf = true;
        sRecord.skipPdfRay = Ray(record.point, reflected, rayIn.Time());

        return true;
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

    bool Scatter(const Ray &rayIn, const HitRecord &record, ScatterRecord &sRecord) const override
    {
        sRecord.attenuation = Colour(1.0, 1.0, 1.0);
        sRecord.pdfPtr = nullptr;
        sRecord.skipPdf = true;
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

        sRecord.skipPdfRay = Ray(record.point, direction, rayIn.Time());
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

    bool Scatter(const Ray &rayIn, const HitRecord &record, ScatterRecord &sRecord) const override
    {
        return false;
    }

    Colour Emitted(const Ray &rayIn, const HitRecord &record, double u, double v, const Point3 &p) const override
    {
        if ( !record.frontFace ) return Colour(0, 0, 0);
        return emit->Value(u, v, p);
    }
};

class Isotropic : public Material
{
private:
    shared_ptr<Texture> tex;

public:
    Isotropic(Colour c) : tex(make_shared<SolidColour>(c)) {}

    Isotropic(shared_ptr<Texture> a) : tex(a) {}

    bool Scatter(const Ray &rayIn, const HitRecord &record, ScatterRecord &sRecord) const override
    {
        sRecord.attenuation = tex->Value(record.u, record.v, record.point);
        sRecord.pdfPtr = make_shared<SpherePDF>();
        sRecord.skipPdf = false;
        return true;
    }

    double ScatteringPDF(const Ray &rayIn, const HitRecord &record, const Ray &scattered) const override
    {
        return 1 / (4 * PI);
    }
};

#endif