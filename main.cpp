#include "rtweekend.h"

#include "camera.h"
#include "hitableList.h"
#include "sphere.h"

int main()
{
    // World
    HitableList world;

    world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.Render(world);

    return 0;
}