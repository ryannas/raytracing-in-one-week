#ifndef RAY_H
#define RAY_H

#include "vec.h"

class Ray{
public:
    Vec3 origin;
    Vec3 dir;

    Ray() {};
    Ray(const Vec3& o, const Vec3& d)
        :origin(o), dir(d) {
    }

    Vec3 at(double t) const {
        return origin + t * dir;
    }
};

#endif
