cmake_minimum_required(VERSION 3.12)
project(Pathtracer)

set(CMAKE_CXX_STANDARD 14)


add_executable(Pathtracer main.cpp vec3.h ray.h hitable.h hitable_list.h sphere.h camera.h material.h math_helper.h)
