// UNIkeEN 2023/11/22

#pragma once

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
    }
}