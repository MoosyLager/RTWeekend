#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Colour Value(double u, double v, const Point3 &p) const = 0;
};

class SolidColour : public Texture
{
private:
    Colour colourValue;

public:
    SolidColour(Colour c) : colourValue(c) {}

    SolidColour(double red, double green, double blue)
        : SolidColour(Colour(red, green, blue)) {}

    Colour Value(double u, double v, const Point3 &p) const override
    {
        return colourValue;
    }
};

class CheckerTexture : public Texture
{
private:
    double inverseScale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;

public:
    CheckerTexture(double _scale, shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
        : inverseScale(1.0 / _scale), even(_even), odd(_odd) {}

    CheckerTexture(double _scale, Colour c1, Colour c2)
        : inverseScale(1.0 / _scale),
          even(make_shared<SolidColour>(c1)),
          odd(make_shared<SolidColour>(c2)) {}

    Colour Value(double u, double v, const Point3 &p) const override
    {
        auto xInteger = static_cast<int>(std::floor(inverseScale * p.X()));
        auto yInteger = static_cast<int>(std::floor(inverseScale * p.Y()));
        auto zInteger = static_cast<int>(std::floor(inverseScale * p.Z()));

        bool isEven = ((xInteger + yInteger + zInteger) & 2) == 0;

        return isEven ? even->Value(u, v, p) : odd->Value(u, v, p);
    }
};

#endif