#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "ppm.h"
#include "camera.h"
#include "material.h"

#include <tbb/tbb.h>
#include <iostream>

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
            Ray scattered;
            color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * rayCast(scattered, world, depth - 1);
            return {0, 0, 0};
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
    Camera camera(point3(-2,2,1), point3(0,0,-1), Vec3(0,1,0), 20.0, 16.0 / 9.0);

    // prepare frame
    const auto aspect_ratio = camera.getAspectRatio();
    const int image_width = 400;
    const int image_height = image_width / aspect_ratio;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // prepare material
    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.7, 0.7, 0.3));
    auto material_left   = make_shared<Dielectric>(1.5);
    auto material_right  = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.2);

    // construct world
    HittableList world;
    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));

    // construct image
    PPM image(image_width, image_height, samples_per_pixel);

    // tbb accelerate
    parallel_for(blocked_range2d<int>(0, image_width, 0, image_height),
            tbb_shading(image, camera, world, image_width, image_height, samples_per_pixel, max_depth));


    image.write_to_file();
    std::cerr << "\nDone.\n";
    return 0;
}
