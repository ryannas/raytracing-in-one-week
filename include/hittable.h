#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    Vec3 p;
    Vec3 n;
    double t;
    bool front_face;

    inline void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
        // opposite with ray means point in the front face
        front_face = r.dir.dot(outward_normal) < 0;
        n = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif
