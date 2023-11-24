//
// Created by UNIkeEN on 23-11-22.
//
#pragma once

#include "ray.h"
#include "intersection.h"

class Object {
    public:
        virtual ~Object() = default;

        virtual bool intersect(const Ray&, double, double, Intersection&) const = 0;
};