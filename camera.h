#ifndef CAMERA_H
#define CAMERA_H

#include <execution>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "rtweekend.h"

#include "colour.h"
#include "hitable.h"
#include "stbImplementation.h"

class Camera
{
private:
    int imageHeight;
    Point3 centre;
    Point3 pixelZeroLoc;
    Vec3 pixelDeltaU;
    Vec3 pixelDeltaV;

    static const int imageComponents = 3;
    uint8_t *image;

    std::vector<int> horizontalImageIter;
    std::vector<int> verticalImageIter;
    std::vector<int> samplesIter;

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

        horizontalImageIter.resize(imageWidth);
        verticalImageIter.resize(imageHeight);
        samplesIter.resize(samplesPerPixel);
        for ( int i = 0; i < imageWidth; i++ ) {
            horizontalImageIter[i] = i;
        }
        for ( int i = 0; i < imageHeight; i++ ) {
            verticalImageIter[i] = i;
        }
        for ( int i = 0; i < samplesPerPixel; i++ ) {
            samplesIter[i] = i;
        }

        image = new uint8_t[imageWidth * imageHeight * imageComponents];
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

    Colour RayColour(const Ray &ray, int depth, const Hitable &world)
    {
        HitRecord record;

        // If we've exceeded the ray bounce limit, no more light is gathered
        if ( depth <= 0 ) return Colour(0, 0, 0);

        if ( world.Hit(ray, Interval(0, INF), record) ) {
            Vec3 direction = RandomOnHemisphere(record.normal);
            return 0.5 * RayColour(Ray(record.point, direction), depth - 1, world);
        }

        Vec3 unitDirection = UnitVector(ray.Direction());
        auto a = 0.5 * (unitDirection.Y() + 1.0);
        return (1.0 - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
    }

public:
    double aspectRatio = 1.0; // Ratio of image width over height
    int imageWidth = 100;     // Rendered image width in pixel count
    int samplesPerPixel = 10; // Count of random samples for each pixel
    int maxDepth = 10;        // Maximum number of ray bounces into scene
    void Render(const Hitable &world)
    {
        Initialise();

        std::for_each(std::execution::par, verticalImageIter.begin(), verticalImageIter.end(), [this, &world](int j) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            std::for_each(std::execution::par, horizontalImageIter.begin(), horizontalImageIter.end(), [this, j, &world](int i) {
                Colour pixelColour(0, 0, 0);
                std::for_each(std::execution::par, samplesIter.begin(), samplesIter.end(), [this, j, i, &world, &pixelColour](int s) {
                    Ray ray = GetRay(i, j);
                    pixelColour += RayColour(ray, maxDepth, world);
                });
                int pixelIndex = 3 * (j * imageWidth + i);
                WriteColour(image, pixelIndex, pixelColour, samplesPerPixel);
            });
        });

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

        stbi_write_png(filename.c_str(), imageWidth, imageHeight, imageComponents, image, imageWidth * imageComponents);
        delete[] image;

        std::clog << "\rDone.                 \n";
    }
};

#endif