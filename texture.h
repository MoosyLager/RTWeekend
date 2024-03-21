#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "rtweekend.h"
#include "stbImplementation.h"

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

class ImageTexture : public Texture
{
private:
    RTWImage image;

public:
    ImageTexture(const char *filename) : image(filename) {}

    Colour Value(double u, double v, const Point3 &p) const override
    {
        // If we haave no texture data, then reutrn soild cyan as a debugging aid.
        if ( image.Height() <= 0 ) return Colour(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [0,1]
        u = Interval(0, 1).Clamp(u);
        v = 1.0 - Interval(0, 1).Clamp(v); // Flip V to image coordinates

        auto i = static_cast<int>(u * image.Width());
        auto j = static_cast<int>(v * image.Height());
        auto pixel = image.pixelData(i, j);

        auto colourScale = 1.0 / 255.0;
        return Colour(colourScale * pixel[0], colourScale * pixel[1], colourScale * pixel[2]);
    }
};

class NoiseTexture : public Texture
{
private:
    Perlin noise;

public:
    NoiseTexture() {}

    Colour Value(double u, double v, const Point3 &p) const override
    {
        return Colour(1, 1, 1) * noise.Noise(p);
    }
};

#endif