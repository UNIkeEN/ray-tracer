//
// Created by unikeen on 23-11-25.
//

#pragma once

#include "vector3.h"
#include "utils.h"
#include "ray.h"

class Bbox {
    public:
        Vector3 pmin, pmax;

        Bbox() = default;

        Bbox(const Vector3& _pmin, const Vector3& _pmax): pmin(_pmin), pmax(_pmax) {}

        bool intersect(const Ray& r, double tmin, double tmax) const {
            Vector3 origin = r.origin;
            Vector3 direction = r.direction;

            for (int i = 0; i < 3; i++) {
                auto t0 = (pmin[i] - origin[i]) / direction[i];
                auto t1 = (pmax[i] - origin[i]) / direction[i];

                if (direction[i] < 0) std::swap(t0, t1);

                if (t0 > tmin) tmin = t0;
                if (t1 < tmax) tmax = t1;

                if (tmax <= tmin) return false;
            }
            return true;
        }

};