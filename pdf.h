#ifndef PDF_H
#define PDF_H

#include "hitableList.h"
#include "onb.h"
#include "rtweekend.h"

class PDF
{
public:
    virtual ~PDF() {}

    virtual double Value(const Vec3 &direction) const = 0;
    virtual Vec3 Generate() const = 0;
};

class SpherePDF : public PDF
{
public:
    SpherePDF() {}

    double Value(const Vec3 &direction) const override
    {
        return 1 / (4 * PI);
    }

    Vec3 Generate() const override
    {
        return RandomUnitVector();
    }
};

class CosinePDF : public PDF
{
private:
    ONB uvw;

public:
    CosinePDF(const Vec3 &w) : uvw(w) {}

    double Value(const Vec3 &direction) const override
    {
        auto cosineTheta = Dot(UnitVector(direction), uvw.W());
        return std::fmax(0, cosineTheta / PI);
    }

    Vec3 Generate() const override
    {
        return uvw.Transform(RandomCosineDirection());
    }
};

class HitablePDF : public PDF
{
private:
    const Hitable &objects;
    Point3 origin;

public:
    HitablePDF(const Hitable &objects, const Point3 &origin) : objects(objects), origin(origin) {}

    double Value(const Vec3 &direction) const override
    {
        return objects.PDFValue(origin, direction);
    }

    Vec3 Generate() const override
    {
        return objects.Random(origin);
    }
};

#endif