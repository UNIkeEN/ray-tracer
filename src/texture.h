//
// Created by unikeen on 23-12-4.
//

#pragma once

#include "utils.h"
#include "vector.h"
#include "external/rtw_stb_image.h"

#include <memory>

class Texture {
    public:
        virtual ~Texture() = default;

        virtual Vector3 get_color(const double& u, const double& v, const Vector3& p) const {
            return {0, 0, 0};
        }
};

class SolidColor : public Texture {
    public:
        SolidColor(const Vector3& _color): color(_color) {}

        Vector3 get_color(const double& u, const double& v, const Vector3& p) const override {
            return color;
        }

    private:
        Vector3 color;
};

class CheckerTexture : public Texture {
    public:
        // use shared_ptr to auto destruct
        CheckerTexture(const double& _scale, std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
            : scale(_scale), even(_even), odd(_odd) {}
        CheckerTexture(const double& _scale, Vector3 _even, Vector3 _odd)
            : scale(_scale), even(std::make_shared<SolidColor>(_even)), odd(std::make_shared<SolidColor>(_odd)) {}

//      CheckerTexture(Texture* _even, Texture* _odd) : even(_even), odd(_odd) {}
//      CheckerTexture(Vector3 _even, Vector3 _odd) :
//          even(new SolidColor(_even)), odd(new SolidColor(_odd)) {}

        Vector3 get_color(const double& u, const double& v, const Vector3& p) const override {
            auto x = static_cast<int>(std::floor(scale * p.x));
            auto y = static_cast<int>(std::floor(scale * p.y));
            auto z = static_cast<int>(std::floor(scale * p.z));

            bool flag = (x + y + z) % 2 == 0;

            return flag ? even->get_color(u, v, p) : odd->get_color(u, v, p);
        }

    private:
        double scale;
        std::shared_ptr<Texture> even;
        std::shared_ptr<Texture> odd;
};

// texture of "missing texture" for debugging
CheckerTexture missing_texture = {0.3, {0.667, 0, 0.667}, {0, 0, 0}};

class ImageTexture : public Texture {
    public:
        ImageTexture(const char* filename) : image(filename) {}

        Vector3 get_color(const double& u, const double& v, const Vector3& p) const override {
            // no texture data
            if (image.height() <= 0) return missing_texture.get_color(u, v, p);

            // clamp input uv coordinates to [0,1] x [1,0] (image coordinates)
            double _u = clamp(u, 0, 1);
            double _v = 1.0 - clamp(v, 0, 1);

            auto i = static_cast<int>(_u * image.width());
            auto j = static_cast<int>(_v * image.height());
            auto pixel = image.pixel_data(i,j);

            return {pixel[0] / 255.0, pixel[1] / 255.0, pixel[2] / 255.0};
        }

    private:
        rtw_image image;
};