#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include "stbImplementation.h"

using Colour = Vec3;

inline double LinearToGamma(double linearComponent)
{
    return sqrt(linearComponent);
}

void WriteColour(uint8_t *image, int pixelIndex, Colour pixelColour, int samplesPerPixel)
{

    auto R = pixelColour.X();
    auto G = pixelColour.Y();
    auto B = pixelColour.Z();

    // Replace NaN components with 0.
    if ( R != R ) R = 0.0;
    if ( G != G ) G = 0.0;
    if ( B != B ) B = 0.0;

    // Divide the colours by the number of samples
    auto scale = 1.0 / samplesPerPixel;
    R *= scale;
    G *= scale;
    B *= scale;

    // Apply the linear to gamma transform
    R = LinearToGamma(R);
    G = LinearToGamma(G);
    B = LinearToGamma(B);

    // Write the translated [0,255] value of each colour component
    static const Interval intensity(0.000, 0.999);
    int IR = static_cast<int>(256 * intensity.Clamp(R));
    int IG = static_cast<int>(256 * intensity.Clamp(G));
    int IB = static_cast<int>(256 * intensity.Clamp(B));

    image[pixelIndex] = IR;
    image[pixelIndex + 1] = IG;
    image[pixelIndex + 2] = IB;
}

#endif