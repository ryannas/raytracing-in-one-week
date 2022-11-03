#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "vec.h"

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535;

inline double degrees_to_radians(double degrees) {
    return  degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline Vec3 random_vec3() {
    return Vec3(random_double(), random_double(), random_double());
}

inline Vec3 random_vec3(double min, double max) {
    return Vec3(
        random_double(min, max),
        random_double(min, max),
        random_double(min, max)
    );
}

Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random_vec3(-1, 1);
        if (p.length_sqrd() >= 1) continue;
        return p;
    }
}

Vec3 random_unit_vector() {
    return random_in_unit_sphere().normalized();
}

Vec3 random_in_hemisphere(const Vec3& normal) {
    auto in_unit_sphere = random_in_unit_sphere();
    if (normal.dot(in_unit_sphere) > 0.0)
        return in_unit_sphere;
    return -in_unit_sphere;
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#include "ray.h"
#include "vec.h"

using point3 = Vec3;

#endif
