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

    Camera(const point3& from, const point3& to, const Vec3& p, double f, double ar)
        : fov(f), aspect_ratio(ar), lookFrom(from), lookTo(to), up(p) {
        auto theta = degree_to_radians(fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = (lookFrom - lookTo).normalized();
        auto u = up.cross(w);
        auto v = w.cross(u);

        origin = lookFrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }
    
    Ray shootRay(double u, double v) {
        return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
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
    double fov;
    double aspect_ratio;
};

#endif
