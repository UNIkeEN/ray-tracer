//
// Created by unikeen on 23-12-4.
//

#pragma

#include "object.h"
#include "bbox.h"

#include <memory>

class BVHNode : public Object {
    public:

        Bbox bbox;
        std::shared_ptr<Object> left;
        std::shared_ptr<Object> right;

        BVHNode(): left(nullptr), right(nullptr) {}

        BVHNode(std::shared_ptr<Object> _left, std::shared_ptr<Object> _right, const Bbox& _bbox):
            left(_left), right(_right), bbox(_bbox) {}

        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
            if (!bbox.intersect(r, tmin, tmax))
                return false;

            bool hit_left = left && left->intersect(r, tmin, tmax, rec);
            bool hit_right = right && right->intersect(r, tmin, hit_left ? rec.t : tmax, rec);

            return hit_left || hit_right;
        }
};
