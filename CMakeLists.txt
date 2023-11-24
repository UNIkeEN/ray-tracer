cmake_minimum_required(VERSION 3.10)

# 项目名称
project(UNIkeEN_ray_tracer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
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
    src/vector3.h
    src/triangle.h
)

add_executable(UNIkeEN_ray_tracer ${SOURCE})