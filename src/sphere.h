//
// Created by UNIkeEN on 23-11-23.
//

#pragma once

#include "object.h"
#include "vector3.h"
#include "ray.h"
#include "utils.h"

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
            Vector3 uni_normal = (rec.p - center) / radius;
            rec.set_correct_normal(r, uni_normal);
            rec.mat = mat;
            get_uv(uni_normal, rec.u, rec.v);

            return true;
        }

    private:
        static void get_uv(const Vector3& p, double& u, double& v) {
            // p is relative Cartesian coordinates on unit sphere
            auto phi = acos(-p.y);
            auto theta = atan2(-p.z, p.x) + PI;

            // uv is [0,1] x [0,1]
            u = theta / (2.0 * PI);
            v = phi / PI;
        }
};