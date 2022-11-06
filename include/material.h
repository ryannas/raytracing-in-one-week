#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

struct HitRecord;

class Material {
public:
    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
            color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    color albedo;

    explicit Lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                         color& attenuation, Ray& scattered) const override {
        auto scatter_dir = rec.n + random_unit_vector();
        if (scatter_dir.near_zero()) scatter_dir = rec.n;
        scattered = Ray(rec.p, scatter_dir);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material {
public:
    color albedo;
    double fuzz;

    Metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                         color& attenuation, Ray& scattered) const override {
        auto reflected = reflect(r_in.dir.normalized(), rec.n);
        reflected += fuzz * random_in_unit_sphere();
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return (rec.n.dot(scattered.dir) > 0);
    }
};

#endif //MATERIAL_H
