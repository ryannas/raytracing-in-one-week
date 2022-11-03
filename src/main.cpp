#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "ppm.h"
#include "camera.h"

#include <tbb/tbb.h>
#include <iostream>
#include <chrono>

using namespace tbb::detail::d1;
using namespace std::chrono;

struct tbb_shading{
    PPM& image;
    Camera& camera;
    HittableList& scene;

    int width;
    int height;
    int samples_per_pixel;
    int max_depth;

    tbb_shading(PPM& p, Camera& c, HittableList& s, int w, int h, int sp, int m)
        : image(p)
        , camera(c)
        , scene(s)
        , width(w)
        , height(h)
        , samples_per_pixel(sp)
        , max_depth(m){

    }

    [[nodiscard]] static color rayCast(const Ray& r, const HittableList& world, int depth)  {
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

    void operator() (const blocked_range2d<int>& r) const {
        for (int i=r.rows().begin(); i!=r.rows().end(); ++i) {
            for (int j=r.cols().begin(); j!=r.cols().end(); ++j ) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (width - 1);
                    auto v = (j + random_double()) / (height - 1);
                    Ray ray = camera.shootRay(u, v);
                    pixel_color += rayCast(ray, scene, max_depth);
                }
                image.shade(i, j, pixel_color);
            }
        }
    }
};

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

    // construct image
    PPM image(image_width, image_height, samples_per_pixel);

    // tbb accelerate
    auto row_begin = image.ppm.begin();
    auto row_end = image.ppm.end();

    system_clock::time_point start, end;
    start = system_clock::now();
    parallel_for(blocked_range2d<int>(0, image_width, 0, image_height),
            tbb_shading(image, camera, world, image_width, image_height, samples_per_pixel, max_depth));
    end = system_clock::now();
    std::cout << "tbb: " << duration_cast<milliseconds>(end-start).count() << " milliseconds" << std::endl;

    start = system_clock::now();
    for (int j = image_height - 1; j >= 0; --j) {
        // std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = camera.shootRay(u, v);
                pixel_color += tbb_shading::rayCast(r, world, max_depth);
            }
            image.shade(i, j, pixel_color);
        }
    }
    end = system_clock::now();
    std::cout << "serial: " << duration_cast<milliseconds>(end-start).count() << " milliseconds" << std::endl;

    image.write_to_file();
    std::cerr << "\nDone.\n";
    return 0;
}
