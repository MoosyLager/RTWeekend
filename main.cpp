#include "rtweekend.h"

#include "camera.h"
#include "colour.h"
#include "hitableList.h"
#include "material.h"
#include "sphere.h"

void FinalRenderBookOne()
{
    // World
    HitableList world;

    auto groundMaterial = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    for ( int i = -11; i < 11; i++ ) {
        for ( int j = -11; j < 11; j++ ) {
            auto chooseMaterial = RandomDouble();
            Point3 centre(i + 0.9 * RandomDouble(), 0.2, j + 0.9 * RandomDouble());

            if ( (centre - Point3(4, 0.2, 0)).Length() > 0.9 ) {
                shared_ptr<Material> sphereMaterial;

                if ( chooseMaterial < 0.8 ) {
                    // diffuse
                    auto albedo = Colour::Random() * Colour::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    auto centre2 = centre + Vec3(0, RandomDouble(0, 0.5), 0);
                    world.Add(make_shared<Sphere>(centre, centre2, 0.2, sphereMaterial));
                } else if ( chooseMaterial < 0.95 ) {
                    // metal
                    auto albedo = Colour::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDistance = 10.0;

    cam.Render(world);
}

void RandomSpheres()
{
    // World
    HitableList world;

    auto groundMaterial = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    for ( int i = -11; i < 11; i++ ) {
        for ( int j = -11; j < 11; j++ ) {
            auto chooseMaterial = RandomDouble();
            Point3 centre(i + 0.9 * RandomDouble(), 0.2, j + 0.9 * RandomDouble());

            if ( (centre - Point3(4, 0.2, 0)).Length() > 0.9 ) {
                shared_ptr<Material> sphereMaterial;

                if ( chooseMaterial < 0.8 ) {
                    // diffuse
                    auto albedo = Colour::Random() * Colour::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                } else if ( chooseMaterial < 0.95 ) {
                    // metal
                    auto albedo = Colour::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 8;

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDistance = 10.0;

    cam.Render(world);
}

int main()
{
    switch ( 2 ) {
        case 1:
            FinalRenderBookOne();
            break;
        case 2:
            RandomSpheres();
            break;
    }
    return 0;
}