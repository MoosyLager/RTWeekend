#include <filesystem>
#include <iostream>
#include <string>

#include "EasyBMP.hpp"
#include "colour.h"
#include "ray.h"
#include "stbImplementation.h"
#include "vec3.h"

float HitSphere(const Point3 &centre, float radius, const Ray &ray)
{
    Vec3 oc = ray.Origin() - centre;
    auto a = ray.Direction().LengthSquared();
    auto halfB = Dot(oc, ray.Direction());
    auto c = oc.LengthSquared() - radius * radius;
    auto discriminant = halfB * halfB - a * c;

    if ( discriminant < 0 ) {
        return -1.0f;
    } else {
        return (-halfB - sqrt(discriminant)) / a;
    }
}

Colour RayColour(const Ray &ray)
{
    auto t = HitSphere(Point3(0, 0, -1), 0.5f, ray);
    if ( t > 0.0f ) {
        Vec3 normal = UnitVector(ray.At(t) - Vec3(0, 0, -1));
        return 0.5f * Colour(normal.X() + 1, normal.Y() + 1, normal.Z() + 1);
    }

    Vec3 unitDirection = UnitVector(ray.Direction());
    auto a = 0.5f * (unitDirection.Y() + 1.0f);
    return (1.0f - a) * Colour(1.0, 1.0, 1.0) + a * Colour(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    auto aspectRatio = 16.0f / 9.0f;
    int imageWidth = 256;
    int imageComps = 3;

    // Calculate the image height, and ensure it's at least 1
    int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // Camera
    auto focalLength = 1.0f;
    auto viewportHeight = 2.0f;
    auto viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);
    auto cameraCentre = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    auto viewportU = Vec3(viewportWidth, 0, 0);
    auto viewportV = Vec3(0, -viewportHeight, 0);

    // Calculate the horizontal and vertical ddlta vectors from pixel to pixel
    auto pixelDeltaU = viewportU / imageWidth;
    auto pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of the upper left pixel.
    auto viewportUpperLeft = cameraCentre - Vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
    auto pixelZeroLoc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    // Create image object with black background
    EasyBMP::RGBColor black(0, 0, 0);
    EasyBMP::Image image(imageWidth, imageHeight, "filename.png", black);

    // Render
    int pixelIndex = 0;

    for ( int j = 0; j < imageHeight; j++ ) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for ( int i = 0; i < imageWidth; i++ ) {
            auto pixelCentre = pixelZeroLoc + (i * pixelDeltaU) + (j * pixelDeltaV);
            auto rayDirection = pixelCentre - cameraCentre;
            Ray ray(cameraCentre, rayDirection);

            Colour pixelColour = RayColour(ray);
            WriteColour(image, pixelColour, i, j);
        }
    }
    std::clog << "\rDone.                 \n";

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

    return 0;
}