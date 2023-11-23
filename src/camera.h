// UNIkeEN 2023/11/23

#pragma once

#include <cmath>

#include "intersection.h"
#include "ray.h"
#include "scene.h"
#include "utils.h"
#include "vector3.h"

class Camera {
    public:
        int height = 960;
        int width = 1280;
        double vfov = 40;

        int max_depth = 10; // Maximum number of ray bounces
        int spp = 10;       // Sample per pixel

        Vector3 lookfrom = Vector3(0, 0, -1);
        Vector3 lookat = Vector3(0, 0, 0);
        Vector3 absolute_up = Vector3(0, 1, 0);

        Vector3 background_color = Vector3(0, 0, 0);

        Camera(const int& _height, const int& _width): height(_height), width(_width) {}

    private:
        double theta;
        Vector3 center;
        Vector3 loc_pixel_00;       // Location of (0,0)
        Vector3 vp_pixel_u, vp_pixel_v;   // A pixel in the image corresponds to a step in the viewport
        Vector3 u, v, w;            // Frame basis vector

        void init() {
            center = lookfrom;
            theta = deg2rad(vfov);
            Vector3 gaze = lookat - lookfrom;
            auto vp_height = 2 * tan(theta) * gaze.length();
            auto vp_width = vp_height * (static_cast<double>(width)/height);

            w = uniform(gaze);
            u = uniform(cross(w , absolute_up));
            v = cross(w, u); 

            Vector3 vp_vec_u = vp_width * u;
            Vector3 vp_vec_v = vp_height * v;

            vp_pixel_u = vp_vec_u / width;
            vp_pixel_v = vp_vec_v / height;
            loc_pixel_00 = lookat - vp_vec_u/2 - vp_vec_v/2 + 0.5*(vp_pixel_u + vp_pixel_v);
        }

        Ray generate_ray(int i, int j) const {
            Vector3 vp_pixel_center = loc_pixel_00 + (i * vp_pixel_u) + (j * vp_pixel_v);

            auto ranx = -0.5 + random_double();
            auto rany = -0.5 + random_double();

            Vector3 vp_pixel_sample = vp_pixel_center + (ranx * vp_pixel_u) + (rany * vp_pixel_v);
            
            return Ray(center, vp_pixel_sample - center);
        }

        Vector3 get_ray_color(const Ray& r_in, int bounce_left, const Scene& world) const {
            // check if meet bounce limit
            if (bounce_left <= 0) return Vector3(0, 0, 0);

            Intersection rec;

            if (!world.intersect(r_in, EPSILON, INF, rec)) return background_color;

            //emit

            //scatter

        } 
};