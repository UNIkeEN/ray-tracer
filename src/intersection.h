//
// Created by UNIkeEN on 23-11-22.
//

#pragma once

#include "vector.h"
#include "ray.h"

class Material;

class Intersection {
    public:
        Vector3 p;
        Vector3 normal;
        double t;
        double u, v;
        Material* mat;
        bool front_face;

        void set_correct_normal(const Ray& r, const Vector3& _normal) {
            front_face =  dot(r.direction, _normal) < 0;
            normal = front_face ? _normal : -_normal;
        }
};