cmake_minimum_required(VERSION 3.10)

project(UNIkeEN_ray_tracer)

# need C++ 17 (OBJ_Loader library needs)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set ( SOURCE
    src/intersection.h
    src/main.cpp
    src/material.h
    src/object.h
    src/ray.h
    src/scene.h
    src/sphere.h
    src/utils.h
    src/vector.h
    src/triangle.h
    src/bbox.h
    src/texture.h
    src/external/OBJ_Loader.hpp
    src/external/stb_image.h
    src/external/rtw_stb_image.h
)

add_executable(UNIkeEN_ray_tracer ${SOURCE})