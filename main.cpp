#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "colour.h"
#include "constantMedium.h"
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

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;
    cam.background = Colour(0.70, 0.80, 1.00);

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDistance = 10.0;

    cam.Render(world, lights);
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

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;
    cam.background = Colour(0.70, 0.80, 1.00);

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0.6;
    cam.focusDistance = 10.0;

    cam.Render(world, lights);
}

void TwoSpheres()
{
    HitableList world;

    auto checkerTexture = make_shared<CheckerTexture>(0.8, Colour(.2, .3, .1), Colour(.9, .9, .9));

    world.Add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checkerTexture)));
    world.Add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checkerTexture)));

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = Colour(0.70, 0.80, 1.00);

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
}

void Mars()
{
    auto marsTexture = make_shared<ImageTexture>("mars.jpg");
    auto marsSurface = make_shared<Lambertian>(marsTexture);
    auto planet = make_shared<Sphere>(Point3(0, 0, 0), 2, marsSurface);

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;
    cam.background = Colour(0.70, 0.80, 1.00);

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(0, 0, 12);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(HitableList(planet), lights);
}

void TwoPerlinSpheres()
{
    HitableList world;

    auto perlinTexture = make_shared<NoiseTexture>(4);
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTexture)));
    world.Add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTexture)));

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = Colour(0.70, 0.80, 1.00);

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
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

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;
    cam.background = Colour(0.70, 0.80, 1.00);

    cam.verticalFOV = 80;
    cam.lookFrom = Point3(0, 0, 9);
    cam.lookAt = Point3(0, 0, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
}

void SimpleLight()
{
    HitableList world;

    auto perlinTexture = make_shared<NoiseTexture>(4);
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTexture)));
    world.Add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTexture)));

    auto diffuseLight = make_shared<DiffuseLight>(Colour(4, 4, 4));
    world.Add(make_shared<Sphere>(Point3(0, 7, 0), 2, diffuseLight));
    world.Add(make_shared<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), diffuseLight));

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = Colour(0, 0, 0);

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(26, 3, 6);
    cam.lookAt = Point3(0, 2, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
}

void CornellBox()
{
    HitableList world;

    auto red = make_shared<Lambertian>(Colour(.65, .05, .05));
    auto white = make_shared<Lambertian>(Colour(.73, .73, .73));
    auto green = make_shared<Lambertian>(Colour(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Colour(15, 15, 15));

    world.Add(make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.Add(make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.Add(make_shared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.Add(make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.Add(make_shared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.Add(make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    // Box
    shared_ptr<Hitable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.Add(box1);

    // Glass Sphere
    auto glass = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Point3(190, 90, 190), 90, glass));

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    HitableList lights;
    lights.Add(make_shared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial));
    lights.Add(make_shared<Sphere>(Point3(190, 90, 190), 90, emptyMaterial));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 1000;
    cam.maxDepth = 50;
    cam.background = Colour(0, 0, 0);

    cam.verticalFOV = 40;
    cam.lookFrom = Point3(278, 278, -800);
    cam.lookAt = Point3(278, 278, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
}

void CornellSmoke()
{
    HitableList world;

    auto red = make_shared<Lambertian>(Colour(.65, .05, .05));
    auto white = make_shared<Lambertian>(Colour(.73, .73, .73));
    auto green = make_shared<Lambertian>(Colour(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Colour(7, 7, 7));

    world.Add(make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.Add(make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.Add(make_shared<Quad>(Point3(113, 554, 127), Vec3(330, 0, 0), Vec3(0, 0, 305), light));
    world.Add(make_shared<Quad>(Point3(0, 555, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.Add(make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.Add(make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    shared_ptr<Hitable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));

    shared_ptr<Hitable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));

    world.Add(make_shared<ConstantMedium>(box1, 0.01, Colour(0, 0, 0)));
    world.Add(make_shared<ConstantMedium>(box2, 0.01, Colour(1, 1, 1)));

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = Colour(0, 0, 0);

    cam.verticalFOV = 40;
    cam.lookFrom = Point3(278, 278, -800);
    cam.lookAt = Point3(278, 278, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
}

void FinalRenderBookTwo(int image_width, int samples_per_pixel, int max_depth)
{
    HitableList boxes1;
    auto ground = make_shared<Lambertian>(Colour(0.48, 0.83, 0.53));

    int boxesPerSide = 20;
    for ( int i = 0; i < boxesPerSide; i++ ) {
        for ( int j = 0; j < boxesPerSide; j++ ) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = RandomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.Add(Box(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HitableList world;

    world.Add(make_shared<BVHNode>(boxes1));

    auto light = make_shared<DiffuseLight>(Colour(7, 7, 7));
    world.Add(make_shared<Quad>(Point3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto sphereMaterial = make_shared<Lambertian>(Colour(0.7, 0.3, 0.1));
    world.Add(make_shared<Sphere>(center1, center2, 50, sphereMaterial));

    world.Add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.Add(make_shared<Sphere>(
        Point3(0, 150, 145), 50, make_shared<Metal>(Colour(0.8, 0.8, 0.9), 1.0)));

    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.Add(boundary);
    world.Add(make_shared<ConstantMedium>(boundary, 0.2, Colour(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.Add(make_shared<ConstantMedium>(boundary, .0001, Colour(1, 1, 1)));

    auto marsMaterial = make_shared<Lambertian>(make_shared<ImageTexture>("mars.jpg"));
    world.Add(make_shared<Sphere>(Point3(400, 200, 400), 100, marsMaterial));
    auto perlinTexture = make_shared<NoiseTexture>(10);
    world.Add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(perlinTexture)));

    HitableList boxes2;
    auto white = make_shared<Lambertian>(Colour(.73, .73, .73));
    int numSpheres = 1000;
    for ( int j = 0; j < numSpheres; j++ ) {
        boxes2.Add(make_shared<Sphere>(Point3::Random(0, 165), 10, white));
    }

    world.Add(make_shared<Translate>(
        make_shared<RotateY>(
            make_shared<BVHNode>(boxes2), 15),
        Vec3(-100, 270, 395)));

    world = HitableList(make_shared<BVHNode>(world));

    // Light Sources
    auto emptyMaterial = shared_ptr<Material>();
    Quad lights(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMaterial);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = image_width;
    cam.samplesPerPixel = samples_per_pixel;
    cam.maxDepth = max_depth;
    cam.background = Colour(0, 0, 0);

    cam.verticalFOV = 40;
    cam.lookFrom = Point3(478, 278, -600);
    cam.lookAt = Point3(278, 278, 0);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.Render(world, lights);
}

int main()
{
    switch ( 8 ) {
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
        case 7:
            SimpleLight();
            break;
        case 8:
            CornellBox();
            break;
        case 9:
            CornellSmoke();
            break;
        case 10:
            FinalRenderBookTwo(800, 10000, 40); // For final high resolution render
            break;
        case 11:
            FinalRenderBookTwo(400, 250, 4); // For testing
            break;
    }
    return 0;
}