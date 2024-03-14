#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include "EasyBMP.hpp"

using Colour = Vec3;

void WriteColour(EasyBMP::Image &image, Colour pixelColour, int x, int y)
{
    int IR = static_cast<int>(255.999 * pixelColour.X());
    int IG = static_cast<int>(255.999 * pixelColour.Y());
    int IB = static_cast<int>(255.999 * pixelColour.Z());
    EasyBMP::RGBColor pixel(IR, IG, IB);

    image.SetPixel(x, y, pixel);
}

#endif