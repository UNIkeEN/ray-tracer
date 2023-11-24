//
// Created by UNIkeEN on 23-11-23.
//

#pragma once

#include "object.h"

#include <vector>

class Scene : public Object {
    public: 
        std::vector<Object*> objects;

        Scene() {}
        Scene(Object* object) { add(object); }

        void add(Object* object) { objects.push_back(object); }
        void clear() { objects.clear(); }

        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
            Intersection tmp;
            auto if_hit = false;
            double cur_tmax = tmax;

            for (auto& object : objects) {
                if (object->intersect(r, tmin, cur_tmax, tmp)) {
                    if_hit = true;
                    cur_tmax = tmp.t;
                    rec = tmp;
                }
            }

            return if_hit;
        }
        
};