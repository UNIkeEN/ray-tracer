// UNIkeEN 2023/11/22

#pragma once

#include "vector3.h"
#include "ray.h"

class Intersection {
    public:
        Vector3 p;
        Vector3 normal;
        double t;

        /*设置正确的法线方向（与光线点积小于0）*/
        void set_correct_normal(const Ray& r, const Vector3& _normal) {
            normal = dot(r.direction, normal) < 0 ? _normal : -_normal;
        }
};