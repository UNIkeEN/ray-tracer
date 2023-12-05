//
// Created by UNIkeEN on 23-11-22.
//

#pragma once

#include "utils.h"

#include <cmath>

class Vector3 {
    public:
        double x, y, z;
        Vector3(): x(0), y(0), z(0) {}
        Vector3(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
        
        Vector3 operator + (const Vector3 &v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
        Vector3 operator - (const Vector3 &v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
        Vector3 operator * (const Vector3 &v) const { return Vector3(x * v.x, y * v.y, z * v.z); }

        Vector3 operator - () const { return Vector3(-x, -y, -z); }
        Vector3 operator * (const double &r) const { return Vector3(x * r, y * r, z * r); } 
        Vector3 operator / (const double &r) const { return Vector3(x / r, y / r, z / r); } 

        Vector3& operator += (const Vector3 &v) { x += v.x, y += v.y, z += v.z; return *this; }

        double operator[](int index) const { return index == 0 ? x : (index == 1 ? y : z); }
        
        double length2() const { return (x * x + y * y + z * z); }
        double length() const { return sqrt(length2()); }
};

inline Vector3 operator * (const double &r, const Vector3 &v) { return v * r; }

inline double dot(const Vector3 &u, const Vector3 &v) {
    return (u.x * v.x + u.y * v.y + u.z * v.z);
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return Vector3(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    );
}

inline Vector3 uniform(const Vector3 &u) {
    if (u.length2() != 0) {
        double len = u.length();
        return Vector3(u.x / len, u.y / len, u.z / len);
    } else {
        return u;
    }
}

inline Vector3 random_vec(const double& min, const double& max) {
    return Vector3(
        random_double(min, max),
        random_double(min, max),
        random_double(min, max)
    );
}

inline Vector3 min(const Vector3& u, const Vector3& v) {
    return Vector3(std::min(u.x, v.x), std::min(u.y, v.y), std::min(u.z, v.z));
}

inline Vector3 max(const Vector3& u, const Vector3& v) {
    return Vector3(std::max(u.x, v.x), std::max(u.y, v.y), std::max(u.z, v.z));
}

class Vector2 {
    public:
        double x, y;
        Vector2(): x(0), y(0) {}
        Vector2(double _x, double _y): x(_x), y(_y) {}

        Vector2 operator + (const Vector2 &v) const { return Vector2(x + v.x, y + v.y); }
        Vector2 operator * (const double &r) const { return Vector2(x * r, y * r); }
};

inline Vector2 operator * (const double &r, const Vector2 &v) { return v * r; }