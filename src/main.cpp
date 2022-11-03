#include "../include/rtweekend.h"

#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"

#include "../include/camera.h"

#ifdef USE_TBB
#include <tbb/parallel_for.h>
#endif

#include <iostream>

color rayCast(const Ray& r, const HittableList& world, int depth) {
    HitRecord rec;
    if (depth <= 0) return {0, 0, 0};
    if (world.hit(r, 0.001, infinity, rec)) {
        point3 target = rec.p + rec.n + random_unit_vector();
        return 0.5 * rayCast(Ray(rec.p, target - rec.p), world, depth - 1);
    }
    Vec3 u_dir = r.dir.normalized();
    auto t = 0.5 * (u_dir.y + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
} 

int main() {
    // construct camera
    Camera camera;
    
    // prepare frame
    const auto aspect_ratio = camera.getAspectRatio();
    const int image_width = 400;
    const int image_height = image_width / aspect_ratio;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // construct world
    HittableList world;
    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = camera.shootRay(u, v);
                pixel_color += rayCast(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}
