#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "rtweekend.h"

#include "hitable.h"
#include "material.h"
#include "texture.h"

class ConstantMedium : public Hitable
{
private:
    shared_ptr<Hitable> boundary;
    double negativeInverseDensity;
    shared_ptr<Material> phaseFunction;

public:
    ConstantMedium(shared_ptr<Hitable> b, double d, shared_ptr<Texture> a)
        : boundary(b), negativeInverseDensity(-1 / d), phaseFunction(make_shared<Isotropic>(a)) {}

    ConstantMedium(shared_ptr<Hitable> b, double d, Colour c)
        : boundary(b), negativeInverseDensity(-1 / d), phaseFunction(make_shared<Isotropic>(c)) {}

    bool Hit(const Ray &ray, Interval rayT, HitRecord &record) const override
    {
        // Print occasional samples when debugging. To enable, set enableDebug to true.
        const bool enableDebug = false;
        const bool debugging = enableDebug && RandomDouble() < 0.00001;

        HitRecord record1, record2;

        if ( !boundary->Hit(ray, Interval(-maxDouble, +maxDouble), record1) ) return false;

        if ( !boundary->Hit(ray, Interval(record1.t + 0.0001, maxDouble), record2) ) return false;

        if ( debugging ) std::clog << "\nrayTMin=" << record1.t << ", rayTMax=" << record2.t << '\n';

        if ( record1.t < rayT.min ) record1.t = rayT.min;
        if ( record2.t > rayT.max ) record2.t = rayT.max;

        if ( record1.t >= record2.t ) return false;

        if ( record1.t < 0 ) record1.t = 0;

        auto rayLength = ray.Direction().Length();
        auto distanceInsideBoundary = (record2.t - record1.t) * rayLength;
        auto hitDistance = negativeInverseDensity * log(RandomDouble());

        if ( hitDistance > distanceInsideBoundary ) return false;

        record.t = record1.t + hitDistance / rayLength;
        record.point = ray.At(record.t);

        if ( debugging ) {
            std::clog << "hitDistance = " << hitDistance << '\n'
                      << "record.t = " << record.t << '\n'
                      << "record.p = " << record.point << '\n';
        }

        record.normal = Vec3(1, 0, 0); // Arbitrary
        record.frontFace = true;
        record.material = phaseFunction;

        return true;
    }

    AABB BoundingBox() const override { return boundary->BoundingBox(); }
};

#endif