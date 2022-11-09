#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class Camera {
public:
    Camera() {
        aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = Vec3(viewport_width, 0.0, 0.0);
        vertical = Vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0.0, 0.0, focal_length);
    }

    Camera(const point3& from, const point3& to, const Vec3& p, double f, double ar, double ap, double fd)
        : fov(f), aspect_ratio(ar), lookFrom(from), lookTo(to), up(p), aperture(ap), focus_disk(fd) {
        auto theta = degree_to_radians(fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = (lookFrom - lookTo).normalized();
        u = up.cross(w);
        v = w.cross(u);

        origin = lookFrom;
        horizontal = focus_disk * viewport_width * u;
        vertical = focus_disk * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_disk * w;

        lens_radius = aperture / 2;
    }

    
    Ray shootRay(double s, double t) {
        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = u * rd.x + v * rd.y;

        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

    double getAspectRatio() {
        return aspect_ratio;
    }
private:
    point3 lookFrom;
    point3 lookTo;
    Vec3 up;

    point3 origin;
    point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double fov;
    double aspect_ratio;

    double aperture;
    double focus_disk;
    double lens_radius;
};

#endif
