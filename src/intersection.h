//
// Created by UNIkeEN on 23-11-22.
//

#pragma once

#include "vector3.h"
#include "ray.h"

class Material;

class Intersection {
    public:
        Vector3 p;
        Vector3 normal;
        double t;
        double u, v;
        Material* mat;

        void set_correct_normal(const Ray& r, const Vector3& _normal) {
            normal = dot(r.direction, _normal) < 0 ? _normal : -_normal;
        }
};