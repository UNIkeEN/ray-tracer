//
// Created by UNIkeEN on 23-11-22.
//

#pragma once

#include "vector3.h"

class Ray {
    public:
        Vector3 origin;
        Vector3 direction;

        Ray() = default;
        Ray(const Vector3& ori, const Vector3& dir): origin(ori), direction(dir) {}

        Vector3 operator () (double t) const { return origin + direction * t; }
};