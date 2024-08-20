#ifndef PDF_H
#define PDF_H

#include "rtweekend.h"

#include "hitableList.h"
#include "onb.h"

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

class MixturePDF : public PDF
{
private:
    shared_ptr<PDF> p[2];

public:
    MixturePDF(shared_ptr<PDF> p0, shared_ptr<PDF> p1)
    {
        p[0] = p0;
        p[1] = p1;
    }

    double Value(const Vec3 &direction) const override
    {
        return 0.5 * p[0]->Value(direction) + 0.5 * p[1]->Value(direction);
    }

    Vec3 Generate() const override
    {
        if ( RandomDouble() < 0.5 ) {
            return p[0]->Generate();
        } else {
            return p[1]->Generate();
        }
    }
};

#endif