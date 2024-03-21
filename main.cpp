#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "colour.h"
#include "hitableList.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

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
                    auto centre2 = centre + Vec3(0, RandomDouble(0, .5), 0);
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

    world = HitableList(make_shared<BVHNode>(world));

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

void TwoSpheres()
{
    HitableList world;

    auto checkerTexture = make_shared<CheckerTexture>(0.8, Colour(.2, .3, .1), Colour(.9, .9, .9));

    world.Add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checkerTexture)));
    world.Add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checkerTexture)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world);
}

void Mars()
{
    auto marsTexture = make_shared<ImageTexture>("mars.jpg");
    auto marsSurface = make_shared<Lambertian>(marsTexture);
    auto planet = make_shared<Sphere>(Point3(0, 0, 0), 2, marsSurface);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(0, 0, 12);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(HitableList(planet));
}

void TwoPerlinSpheres()
{
    HitableList world;

    auto perlinTexture = make_shared<NoiseTexture>(4);
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTexture)));
    world.Add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTexture)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world);
}

void Quads()
{
    HitableList world;

    // Materials
    auto leftRed = make_shared<Lambertian>(Colour(1.0, 0.2, 0.2));
    auto backGreen = make_shared<Lambertian>(Colour(0.2, 1.0, 0.2));
    auto rightBlue = make_shared<Lambertian>(Colour(0.2, 0.2, 1.0));
    auto upperOrange = make_shared<Lambertian>(Colour(1.0, 0.5, 0.0));
    auto lowerTeal = make_shared<Lambertian>(Colour(0.2, 0.8, 0.8));

    // Quads
    world.Add(make_shared<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), leftRed));
    world.Add(make_shared<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), backGreen));
    world.Add(make_shared<Quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), rightBlue));
    world.Add(make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upperOrange));
    world.Add(make_shared<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lowerTeal));

    world = HitableList(make_shared<BVHNode>(world));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;

    cam.verticalFOV = 80;
    cam.lookFrom = Point3(0, 0, 9);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world);
}

int main()
{
    switch ( 6 ) {
        case 1:
            FinalRenderBookOne();
            break;
        case 2:
            RandomSpheres();
            break;
        case 3:
            TwoSpheres();
            break;
        case 4:
            Mars();
            break;
        case 5:
            TwoPerlinSpheres();
            break;
        case 6:
            Quads();
            break;
    }
    return 0;
}