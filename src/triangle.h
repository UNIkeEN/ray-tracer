//
// Created by UNIkeEN on 23-11-24.
//

#pragma once

#include <vector>
#include <limits>
#include <cassert>
#include <algorithm>

#include "object.h"
#include "vector.h"
#include "ray.h"
#include "bvh.h"
#include "bbox.h"
#include "texture.h"

#include "external/OBJ_Loader.hpp"

class Triangle : public Object {
    public:
        Vector3 v0, v1, v2;
        Vector2 uv0, uv1, uv2;
        Material* mat;
        Bbox bbox;

        Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2, Material* _mat) :
            v0(_v0), v1(_v1), v2(_v2), uv0({0, 0}), uv1({0, 1}), uv2({1, 0}), mat(_mat)
            { init_bbox(); }
        Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2,
                 const Vector2& _uv0, const Vector2& _uv1, const Vector2& _uv2, Material* _mat) :
                v0(_v0), v1(_v1), v2(_v2), uv0(_uv0), uv1(_uv1), uv2(_uv2), mat(_mat) { init_bbox(); }

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
                Vector2 uv = (1 - u - v) * uv0 + u * uv1 + v * uv2;
                rec.u = uv.x;
                rec.v = uv.y;

                return true;
            } else {
                return false;
            }
        }

    private:
        void init_bbox() {
            Vector3 pmin = min(v0, min(v1, v2));
            Vector3 pmax = max(v0, max(v1, v2));
            bbox = Bbox(pmin, pmax);
        }
};

class MeshTriangle : public Object {
    public:
        std::vector<Triangle> triangles;
        Material* mat;
        std::shared_ptr<BVHNode> root;

        MeshTriangle(const std::vector<Triangle>& _triangles) : triangles(_triangles) {
            root = buildBVH(triangles, 0, static_cast<long>(_triangles.size()));
        }

        MeshTriangle(const std::string& filename, Material* _mat): mat(_mat) {
            load_meshes_from_OBJ(filename, false);
            root = buildBVH(triangles, 0, static_cast<long>(triangles.size()));
        }

        MeshTriangle(const std::string& filename) {
            load_meshes_from_OBJ(filename, true);
            root = buildBVH(triangles, 0, static_cast<long>(triangles.size()));
        }

        //with BVH
        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
            return root && root->intersect(r, tmin, tmax, rec);
        }

        //without BVH
//        bool intersect(const Ray& r, double tmin, double tmax, Intersection& rec) const override {
//            Intersection tmp;
//            auto if_hit = false;
//            double cur_tmax = tmax;
//
//            for (const auto& triangle : triangles) {
//                if (triangle.intersect(r, tmin, cur_tmax, tmp)) {
//                    if_hit = true;
//                    cur_tmax = tmp.t;
//                    rec = tmp;
//                }
//            }
//
//            return if_hit;
//        }

        std::shared_ptr<BVHNode> buildBVH(std::vector<Triangle>& triangles_vec, long start, long end) {
            if (start == end - 1)
                return std::make_shared<BVHNode>(
                        std::make_shared<Triangle>(triangles_vec[start]), nullptr, triangles_vec[start].bbox
                );
            if (start == end - 2)
                return std::make_shared<BVHNode>(
                        std::make_shared<Triangle>(triangles_vec[start]), std::make_shared<Triangle>(triangles_vec[start + 1]),
                        Bbox( triangles_vec[start].bbox, triangles_vec[start + 1].bbox )
                );

            // use binary algorithm
            long mid = (start + end) / 2;
            int axios = random_int(0, 2);
            std::nth_element(triangles_vec.begin() + start, triangles_vec.begin() + mid, triangles_vec.begin() + end,
                             [axios](const Triangle& a, const Triangle& b) {
//                                return a.bbox.pmin.z + a.bbox.pmax.z < b.bbox.pmin.z + b.bbox.pmax.z;
                                return a.bbox.pmin[axios] + a.bbox.pmax[axios] < b.bbox.pmin[axios] + b.bbox.pmax[axios];
                             });

            auto left = buildBVH(triangles_vec, start, mid);
            auto right = buildBVH(triangles_vec, mid, end);

            // calc merged bbox
            Bbox bbox = { left->bbox, right->bbox };
            return std::make_shared<BVHNode>(left, right, bbox);
        }

    private:
        void load_meshes_from_OBJ(const std::string& filename, const bool& load_texture) {
            objl::Loader loader;
            bool loaded = loader.LoadFile(filename);
            assert(loaded); // check if loaded successfully

            for (const auto &mesh: loader.LoadedMeshes) {
                for (int i = 0; i < mesh.Indices.size(); i += 3) {
                    Vector3 face_vertices[3];
                    Vector2 face_uvs[3];
                    for (int j = 0; j < 3; j++) {
                        auto vert_idx = mesh.Indices[i + j];
                        auto vert = mesh.Vertices[vert_idx];

                        auto vertex_position = Vector3(vert.Position.X, vert.Position.Y, vert.Position.Z);
                        face_vertices[j] = vertex_position;

                        if (vert_idx < mesh.Vertices.size() && vert_idx < mesh.Vertices.size()) {
                            auto uv = Vector2(vert.TextureCoordinate.X, vert.TextureCoordinate.Y);
                            face_uvs[j] = uv;
                        } else {
                            face_uvs[j] = Vector2(0.0, 0.0);
                        }
                    }

                    triangles.emplace_back(face_vertices[0], face_vertices[1], face_vertices[2],face_uvs[0], face_uvs[1], face_uvs[2],
                       load_texture ? new Lambertian(std::make_shared<ImageTexture>(mesh.MeshMaterial->map_Kd.c_str())) : mat);
                }
            }
        }
};

