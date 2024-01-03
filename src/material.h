//
// Created by UNIkeEN on 23-11-24.
//

#pragma once

#include "intersection.h"
#include "ray.h"
#include "utils.h"
#include "vector.h"
#include "texture.h"

#include <memory>

//specular reflection
inline Vector3 reflect(const Vector3 & v, const Vector3 & n) {
    return v - 2*dot(v,n)*n;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length2())) * n;
    return r_out_perp + r_out_parallel;
}

class Material {
    public:
        
        /* decide scattered color direction
            @param ray_in
            @param rec
            @return bool, attenuation, ray_out*/
        virtual bool scatter(
            const Ray& ray_in, const Intersection& rec, Vector3& attenuation, Ray& ray_out
        ) const = 0;

        // decide emitted color
        virtual Vector3 emit(double u, double v, const Vector3& p) const {
            return Vector3(0, 0, 0);
        }

};

class Lambertian : public Material {
    public:
        Lambertian(Vector3 _albedo): albedo(std::make_shared<SolidColor>(_albedo)) {}
        Lambertian(std::shared_ptr<Texture> _albedo): albedo(_albedo) {}

        bool scatter(const Ray& ray_in, const Intersection& rec, Vector3& attenuation, Ray& ray_out) const override {
            Vector3 random_direction = uniform(random_vec(-1,1));
            Vector3 scatter_direction = rec.normal + random_direction;

            if (dot(random_direction, rec.normal) < (-1 + EPSILON))
                scatter_direction = rec.normal;

            ray_out = Ray(rec.p, scatter_direction);
            attenuation = albedo->get_color(rec.u, rec.v, rec.p);
            return true;
        }

    private:
        std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
    public:
        Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        bool scatter(const Ray& ray_in, const Intersection& rec, Vector3& attenuation, Ray& ray_out) const override {
            Vector3 reflected = reflect(uniform(ray_in.direction), rec.normal);
            ray_out = Ray(rec.p, reflected + fuzz * uniform(random_vec(-1,1)));
            attenuation = albedo;
            return (dot(ray_out.direction, rec.normal) > 0);
        }

    private:
        Vector3 albedo;
        double fuzz;
};

class BasicLight : public Material {
    public:
        bool scatter(const Ray& ray_in, const Intersection& rec, Vector3& attenuation, Ray& ray_out) const override { 
            return false; 
        }

        Vector3 emit(double u, double v, const Vector3& p) const override {
            return Vector3(31.1, 25, 20);
        }
};

class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        bool scatter(const Ray& ray_in, const Intersection& rec, Vector3& attenuation, Ray& ray_out) const override {
            attenuation = Vector3(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            Vector3 unit_direction = uniform(ray_in.direction);
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vector3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            ray_out = Ray(rec.p, direction);
            return true;
        }

    private:
        double ir; // Index of Refraction

        static double reflectance(double cosine, double ref_idx) {
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            return r0 * r0 + (1 - r0 * r0) * pow((1 - cosine), 5);
        }
};