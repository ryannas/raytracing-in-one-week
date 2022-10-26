#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(const Vec3& cen, double r) : center(cen), radius(r) {};

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
    Vec3 center;
    double radius;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.origin - center;
    auto a = r.dir.length_sqrd();
    auto half_b = oc.dot(r.dir);
    auto c = oc.length_sqrd() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.setFaceNormal(r, outward_normal);

    return true;
}

#endif
