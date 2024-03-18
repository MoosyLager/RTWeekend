#include "rtweekend.h"

#include "camera.h"
#include "colour.h"
#include "hitableList.h"
#include "material.h"
#include "sphere.h"

int main()
{
    // World
    HitableList world;

    auto materialGround = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
    auto materialCentre = make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
    auto materialLeft = make_shared<Dielectric>(1.5);
    auto materialRight = make_shared<Metal>(Colour(0.8, 0.6, 0.2), 0.0);

    world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, materialCentre));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.4, materialLeft));
    world.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, materialRight));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;

    cam.verticalFOV = 20;
    cam.lookFrom = Point3(-2, 2, 1);
    cam.lookAt = Point3(0, 0, -1);
    cam.vecUp = Vec3(0, 1, 0);

    cam.defocusAngle = 10.0;
    cam.focusDistance = 3.4;

    cam.Render(world);

    return 0;
}