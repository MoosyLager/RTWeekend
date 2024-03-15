#ifndef CAMERA_H
#define CAMERA_H

#include <filesystem>
#include <iostream>
#include <string>

#include "rtweekend.h"

#include "EasyBMP.hpp"
#include "colour.h"
#include "hitable.h"

class Camera
{
public:
    double aspectRatio = 1.0;
    int imageWidth = 100;
    int samplesPerPixel = 10;

    void Render(const Hitable &world)
    {
        Initialise();

        // Create image object with black background
        EasyBMP::RGBColor black(0, 0, 0);
        EasyBMP::Image image(imageWidth, imageHeight, "filename.png", black);

        for ( int j = 0; j < imageHeight; j++ ) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for ( int i = 0; i < imageWidth; i++ ) {
                Colour pixelColour(0, 0, 0);
                for ( int sample = 0; sample < samplesPerPixel; sample++ ) {
                    Ray ray = GetRay(i, j);
                    pixelColour += RayColour(ray, world);
                }
                WriteColour(image, pixelColour, i, j, samplesPerPixel);
            }
        }

        // Save Image
        std::string path = "C:/Users/shena/Documents/Random Programming Things/Raytracing In One Weekend Series/Images/";

        auto dirIter = std::filesystem::directory_iterator(path.c_str());
        int fileCount = 0;

        for ( auto &entry : dirIter ) {
            if ( std::filesystem::is_regular_file(entry.path()) ) {
                fileCount++;
            }
        }

        std::string filename = path + std::to_string(fileCount + 1) + ".png";

        image.SetFileName(filename);
        image.Write();

        std::clog << "\rDone.                 \n";
    }

private:
    int imageHeight;
    Point3 centre;
    Point3 pixelZeroLoc;
    Vec3 pixelDeltaU;
    Vec3 pixelDeltaV;

    void Initialise()
    {
        imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        centre = Point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focalLength = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewportU = Vec3(viewportWidth, 0, 0);
        auto viewportV = Vec3(0, -viewportHeight, 0);

        // Calculate the horizontal and vertical ddlta vectors from pixel to pixel
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate the location of the upper left pixel.
        auto viewportUpperLeft = centre - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
        pixelZeroLoc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    Ray GetRay(int i, int j) const
    {
        // Get a randomly samples camerea for the pixel at a location i,j.

        auto pixelCentre = pixelZeroLoc + (i * pixelDeltaU) + (j * pixelDeltaV);
        auto pixelSample = pixelCentre + PixelSampleSource();

        auto rayOrigin = centre;
        auto rayDirection = pixelSample - rayOrigin;

        return Ray(rayOrigin, rayDirection);
    }

    Vec3 PixelSampleSource() const
    {
        // Returns a random point in the square surrounding a pixel at the origin
        auto px = -0.5 * RandomDouble();
        auto py = -0.5 * RandomDouble();
        return (px * pixelDeltaU) + (py * pixelDeltaV);
    }

    Colour RayColour(const Ray &ray, const Hitable &world)
    {
        HitRecord record;
        if ( world.Hit(ray, Interval(0, INF), record) ) {
            return 0.5 * (record.normal + Colour(1, 1, 1));
        }

        Vec3 unitDirection = UnitVector(ray.Direction());
        auto a = 0.5 * (unitDirection.Y() + 1.0);
        return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
    }
};

#endif