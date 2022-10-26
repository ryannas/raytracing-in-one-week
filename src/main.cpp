#include "../include/color.h"
#include "../include/ray.h"
#include "../include/vec.h"
#include "../include/sphere.h"

#include <iostream>
#include <cmath>
#include <limits>

color ray_color(const Ray& r) {
    Sphere sphere(Vec3(0, 0, -1), 0.5);
    HitRecord rec;
    if (sphere.hit(r, 0, MAXFLOAT, rec))
        return 0.5 * color(rec.n.x+1, rec.n.y+1, rec.n.z+1);
    Vec3 u_dir = r.dir.normalized();
    auto t = 0.5 * (u_dir.y + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
} 

int main() {
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = image_width / aspect_ratio;

    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    auto origin = Vec3(0, 0, 0);
    auto horizontal = Vec3(viewport_width, 0, 0);
    auto vertical = Vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
    return 0;
}
