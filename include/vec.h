#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <cmath>

class Vec3;
class Vec4;

class Vec3 {
public:
  double x;
  double y;
  double z;

  Vec3() : x(0), y(0), z(0) {};
  
  Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

  Vec3(const Vec3 &v) : x(v.x), y(v.y), z(v.z) {}

  double length_sqrd() { return x * x + y * y + z * z; }

  double length() { return std::sqrt(length_sqrd()); }

  Vec3 operator-() const { return Vec3(-x, -y, -z); }

  Vec3 &operator+=(const Vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vec3 &operator*=(const double t) {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  Vec3 &operator/=(const double t) { return *this *= 1 / t; }

  void normalize() { *this /= length(); }

  Vec3 normalized() {
    Vec3 v(*this);
    v /= length();
    return v;
  }

  double dot(const Vec3 &v) { return x * v.x + y * v.y + z * v.z; }

  Vec3 cross(const Vec3 &v) {
    return Vec3(y * v.z - v.y * z, z * v.x - x * v.z, x * v.y - y * v.x);
  };
};

std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.x << " " << v.y << " " << v.z;
}

Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

Vec3 operator*(const Vec3 &u, double t) {
  return Vec3(u.x * t, u.y * t, u.z * t);
}

Vec3 operator/(const Vec3 &u, double t) { return u * (1 / t); }

Vec3 operator*(double t, const Vec3 &u) { return u * t; }

class Vec4 {
public:
  double x;
  double y;
  double z;
  double w;

  Vec4(double _x, double _y, double _z, double _w)
      : x(_x), y(_y), z(_z), w(_w) {}

  Vec4(const Vec3 &v, double _w) : x(v.x), y(v.y), z(v.z), w(_w) {}

  Vec4(const Vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

  double length_sqrd() { return x * x + y * y + z * z + w * w; }

  double length() { return std::sqrt(length_sqrd()); }

  Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

  Vec4 &operator+=(const Vec4 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  Vec4 &operator*=(const double t) {
    x *= t;
    y *= t;
    z *= t;
    w *= t;
    return *this;
  }

  Vec4 &operator/=(const double t) { return *this *= 1 / t; }

  void normalize() { *this /= length(); }

  Vec4 normalized() {
    Vec4 v(*this);
    v /= length();
    return v;
  }

  double dot(const Vec4 &v) { return x * v.x + y * v.y + z * v.z + w * v.w; }
};

#endif
