//
// Created by UNIkeEN on 23-11-24.
//

#pragma once

#include <vector>
#include <limits>
#include <cassert>

#include "object.h"
#include "vector3.h"
#include "ray.h"

#include "external/OBJ_Loader.hpp"

class Triangle : public Object {
    public:
        Vector3 v0, v1, v2;
        Material* mat;

        Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2, Material* _mat) :
                v0(_v0), v1(_v1), v2(_v2), mat(_mat) {}

        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
            // use Moller-Trumbore intersection algorithm
            // intersection point P = (1-u-v)*v0 + u*v1 + v*v2
            Vector3 edge1 = v1 - v0;
            Vector3 edge2 = v2 - v0;
            Vector3 s1 = cross(r.direction, edge2);
            double det = dot(edge1, s1);
            if (det > -EPSILON && det < EPSILON) return false;  // regard as parallel

            Vector3 s = r.origin - v0;
            double u = dot(s, s1) / det;
            if (u < 0.0 || u > 1.0) return false;

            Vector3 s2 = cross(s, edge1);
            double v = dot(s2, r.direction) / det;
            if (v < 0.0 || u + v > 1.0) return false;

            double t = dot(s2, edge2) / det;
            if (t > tmin && t < tmax) {
                rec.p = r(t);
                rec.t = t;
                rec.set_correct_normal(r, uniform(cross(edge1, edge2)));
                rec.mat = mat;

                // record uv todo

                return true;
            } else {
                return false;
            }
        }
};

class MeshTriangle : public Object {
    public:
        std::vector<Triangle> triangles;
        Material* mat;

        MeshTriangle(const std::vector<Triangle>& _triangles) : triangles(_triangles) {}

        MeshTriangle(const std::string& filename, Material* _mat): mat(_mat) {
            objl::Loader loader;
            bool loaded = loader.LoadFile(filename);
            assert(loaded); // check if loaded successfully

            for (const auto &mesh: loader.LoadedMeshes) {
                for (int i = 0; i < mesh.Indices.size(); i += 3) {
                    Vector3 face_vertices[3];
                    for (int j = 0; j < 3; j++) {
                        auto vert_idx = mesh.Indices[i + j];

                        auto vert = Vector3(mesh.Vertices[vert_idx].Position.X,
                                            mesh.Vertices[vert_idx].Position.Y,
                                            mesh.Vertices[vert_idx].Position.Z);
                        face_vertices[j] = vert;

                    }

                    triangles.emplace_back(face_vertices[0], face_vertices[1],face_vertices[2], mat);
                }
            }
        }

        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
            Intersection tmp;
            auto if_hit = false;
            double cur_tmax = tmax;

            for (const auto& triangle : triangles) {
                if (triangle.intersect(r, tmin, cur_tmax, tmp)) {
                    if_hit = true;
                    cur_tmax = tmp.t;
                    rec = tmp;
                }
            }

            return if_hit;
        }
};

