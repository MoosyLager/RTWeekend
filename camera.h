#ifndef CAMERA_H
#define CAMERA_H

#include <chrono>
#include <execution>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "rtweekend.h"

#include "colour.h"
#include "hitable.h"
#include "material.h"
#include "pdf.h"
#include "stbImplementation.h"

class Camera
{
public:
    double aspectRatio = 1.0; // Ratio of image width over height
    int imageWidth = 100;     // Rendered image width in pixel count
    int samplesPerPixel = 10; // Count of random samples for each pixel
    int maxDepth = 10;        // Maximum number of ray bounces into scene
    Colour background;        // Scene background colour

    double verticalFOV = 90;            // Vertical view angle (field of view)
    Point3 lookFrom = Point3(0, 0, -1); // Point camera is looking from
    Point3 lookAt = Point3(0, 0, 0);    // Point camera is looking at
    Vec3 vecUp = Vec3(0, 1, 0);         // Camera-relative "up" direction

    double defocusAngle = 0;   // Variation angle of rays through each pixel
    double focusDistance = 10; // Distance from camera lookfrom point to plane of perfect focus

    void Render(const Hitable &world, const Hitable &lights)
    {
        Initialise();

        auto startTime = std::chrono::high_resolution_clock::now();

        int scanlinesRemaining = imageHeight;
        std::for_each(std::execution::par_unseq, verticalImageIter.begin(), verticalImageIter.end(), [this, &world, &lights, &scanlinesRemaining](int j) {
            std::clog << "\rScanlines remaining: " << scanlinesRemaining-- << " " << std::flush;
            std::for_each(std::execution::par_unseq, horizontalImageIter.begin(), horizontalImageIter.end(), [this, j, &world, &lights](int i) {
                Colour pixelColour(0, 0, 0);
                std::for_each(std::execution::par_unseq, sqrtSamplesIter.begin(), sqrtSamplesIter.end(), [this, j, i, &world, &lights, &pixelColour](int s_j) {
                    std::for_each(std::execution::par_unseq, sqrtSamplesIter.begin(), sqrtSamplesIter.end(), [this, j, i, &world, &lights, &pixelColour, s_j](int s_i) {
                        Ray ray = GetRay(i, j, s_i, s_j);
                        pixelColour += RayColour(ray, maxDepth, world, lights);
                    });
                });
                int pixelIndex = 3 * (j * imageWidth + i);
                WriteColour(image, pixelIndex, pixelColour, samplesPerPixel);
            });
        });

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime(endTime - startTime);

        // Save Image
        std::string path = "../../Images/Book 3/";
        auto dirIter = std::filesystem::directory_iterator(path.c_str());
        int fileCount = 0;
        for ( auto &entry : dirIter ) {
            if ( std::filesystem::is_regular_file(entry.path()) ) {
                fileCount++;
            }
        }
        std::string filename = path + std::to_string(fileCount + 1) + ".png";

        stbi_write_png(filename.c_str(), imageWidth, imageHeight, imageComponents, image, imageWidth * imageComponents);
        STBI_FREE(image);

        std::clog << "\rDone.                 \n"
                  << std::flush;

        std::clog << "\rRender Time: " << elapsedTime << " " << std::flush;
    }

private:
    int imageHeight;                      // Rendered image height
    int sqrtSamplesPerPixel;              // Square root for a sum of pixel samples
    double reciprocalSqrtSamplesPerPixel; // 1 / sqrtSamplesPerPixel
    Point3 centre;                        // Camera center
    Point3 pixelZeroLoc;                  // Location of pixel 0, 0
    Vec3 pixelDeltaU;                     // Offset to pixel to the right
    Vec3 pixelDeltaV;                     // Offset to pixel below
    Vec3 u, v, w;                         // Camera frame basis vectors
    Vec3 defocusDiskU;                    // Defocus disk horizontal radius
    Vec3 defocusDiskV;                    // Defocus disk vertical radius

    static const int imageComponents = 3;
    uint8_t *image;

    std::vector<int> horizontalImageIter;
    std::vector<int> verticalImageIter;
    std::vector<int> samplesIter;
    std::vector<int> sqrtSamplesIter;

    void Initialise()
    {
        imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        sqrtSamplesPerPixel = int(std::sqrt(samplesPerPixel));
        reciprocalSqrtSamplesPerPixel = 1.0 / sqrtSamplesPerPixel;

        centre = lookFrom;

        // Determine viewport dimensions
        auto theta = DegreesTooRadians(verticalFOV);
        auto h = tan(theta / 2);
        auto viewportHeight = 2 * h * focusDistance;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = UnitVector(lookFrom - lookAt);
        u = UnitVector(Cross(vecUp, w));
        v = Cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewportU = viewportWidth * u;   // Vector across viewport horizontal edge
        auto viewportV = viewportHeight * -v; // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate the location of the upper left pixel.
        auto viewportUpperLeft = centre - (focusDistance * w) - viewportU / 2 - viewportV / 2;
        pixelZeroLoc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        // Calculate the camera defocus disk basis vectors
        auto defocusRadius = focusDistance * tan(DegreesTooRadians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;

        horizontalImageIter.resize(imageWidth);
        verticalImageIter.resize(imageHeight);
        samplesIter.resize(samplesPerPixel);
        sqrtSamplesIter.resize(sqrtSamplesPerPixel);
        for ( int i = 0; i < imageWidth; i++ ) {
            horizontalImageIter[i] = i;
        }
        for ( int i = 0; i < imageHeight; i++ ) {
            verticalImageIter[i] = i;
        }
        for ( int i = 0; i < samplesPerPixel; i++ ) {
            samplesIter[i] = i;
        }
        for ( int i = 0; i < sqrtSamplesPerPixel; i++ ) {
            sqrtSamplesIter[i] = i;
        }

        image = new uint8_t[imageWidth * imageHeight * imageComponents];
    }

    Ray GetRay(int i, int j, int s_i, int s_j) const
    {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j for stratified sample saquare s_i, s_j.

        auto offset = SampleSquareStratified(s_i, s_j);
        auto pixelSample = pixelZeroLoc + ((i + offset.X()) * pixelDeltaU) + ((j + offset.Y()) * pixelDeltaV);

        auto rayOrigin = (defocusAngle <= 0) ? centre : DefocusDiskSample();
        auto rayDirection = pixelSample - rayOrigin;
        auto rayTime = RandomDouble();

        return Ray(rayOrigin, rayDirection, rayTime);
    }

    Vec3 SampleSquareStratified(int s_i, int s_j) const
    {
        // Returns the vector to a random point in the square sub-pixel specified by grid
        // indices s_i and s_j, for an idealised unit square pixel [-0.5, -0.5] to [0.5, 0.5]

        auto px = ((s_i + RandomDouble()) * reciprocalSqrtSamplesPerPixel) - 0.5;
        auto py = ((s_j + RandomDouble()) * reciprocalSqrtSamplesPerPixel) - 0.5;

        return Vec3(px, py, 0);
    }

    Vec3 SampleSquare() const
    {
        // Returns the vector to a random point in the [-0.5, -0.5]-[0.5, 0.5] unit square.
        return Vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
    }

    Vec3 PixelSampleSource() const
    {
        // Returns a random point in the square surrounding a pixel at the origin
        auto px = -0.5 * RandomDouble();
        auto py = -0.5 * RandomDouble();
        return (px * pixelDeltaU) + (py * pixelDeltaV);
    }

    Point3 DefocusDiskSample() const
    {
        // Returns a random point in the camera defocus disk
        auto p = RandomInUnitDisk();
        return centre + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    Colour RayColour(const Ray &ray, int depth, const Hitable &world, const Hitable &lights)
    {
        HitRecord record;

        // If we've exceeded the ray bounce limit, no more light is gathered
        if ( depth <= 0 ) return Colour(0, 0, 0);

        // If the ray hits nothing, return the background colour
        if ( !world.Hit(ray, Interval(0.001, maxDouble), record) ) return background;

        ScatterRecord sRecord;
        Colour colourFromEmission = record.material->Emitted(ray, record, record.u, record.v, record.point);

        if ( !record.material->Scatter(ray, record, sRecord) ) return colourFromEmission;

        if ( sRecord.skipPdf ) {
            return sRecord.attenuation * RayColour(sRecord.skipPdfRay, depth - 1, world, lights);
        }

        auto lightPtr = make_shared<HitablePDF>(lights, record.point);
        MixturePDF p(lightPtr, sRecord.pdfPtr);

        Ray scattered = Ray(record.point, p.Generate(), ray.Time());
        auto pdfValue = p.Value(scattered.Direction());

        double scatteringPDF = record.material->ScatteringPDF(ray, record, scattered);

        Colour sampleColour = RayColour(scattered, depth - 1, world, lights);
        Colour colourFromScatter = (sRecord.attenuation * scatteringPDF * sampleColour) / pdfValue;

        return colourFromEmission + colourFromScatter;
    }
};

#endif