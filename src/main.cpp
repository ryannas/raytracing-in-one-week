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

HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

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
    point3 lookFrom(13,2,2);
    point3 lookAt(0,0,0);
    Vec3 vup(0,1,0);
    auto dist_to_focus = (lookFrom-lookAt).length();
    auto aperture = 0.1;
    auto aspect_ratio = 3.0 / 2.0;

    Camera camera(lookFrom, lookAt, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // prepare frame
    const int image_width = 1200;
    const int image_height = image_width / aspect_ratio;
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // construct world
    HittableList world = random_scene();

    // construct image
    PPM image(image_width, image_height, samples_per_pixel);

    // tbb accelerate
    parallel_for(blocked_range2d<int>(0, image_width, 0, image_height),
            tbb_shading(image, camera, world, image_width, image_height, samples_per_pixel, max_depth));


    image.write_to_file();
    std::cerr << "\nDone.\n";
    return 0;
}
