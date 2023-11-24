//
// Created by UNIkeEN on 23-11-23.
//

#pragma once

#include "object.h"
#include "vector3.h"
#include "ray.h"

#include <cmath>

class Sphere : public Object {
    public:
        Vector3 center;
        double radius;
        Material* mat;

        Sphere(const Vector3& c): center(c), radius(1) {}
        Sphere(const Vector3& c, const double& r, Material* _mat): center(c), radius(r), mat(_mat) {}

        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
            Vector3 o_c = r.origin - center;
            auto a = r.direction.length2();
            auto half_b = dot(r.direction, o_c);
            auto c = o_c.length2() - radius * radius;

            auto discriminant = half_b * half_b - a * c;
            if (discriminant < 0) return false;

            auto sqrt_disc = sqrt(discriminant);
            auto t_near = (-half_b - sqrt_disc) / a;
            if (t_near > tmax) return false;
            if (t_near < tmin) {
                t_near = (-half_b + sqrt_disc) / a;
                if (t_near < tmin || t_near > tmax) return false;
            }

            rec.p = r(t_near);
            rec.t = t_near;
            rec.set_correct_normal(r, (rec.p - center) / radius);
            rec.mat = mat;

            // record uv todo

            return true;
        }
};