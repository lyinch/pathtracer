//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "ray.h"

class camera{
public:
    camera(float vfov, float aspect){
        auto theta = static_cast<float>(vfov * M_PI / 180.0f);
        auto half_height = tanf(theta / 2);
        float half_width = aspect*half_height;
        top_left_corner = Vec3(-half_width,half_height,-1);
        horizontal = Vec3(2*half_width,0,0);
        vertical = Vec3(0,-2*half_height,0);
        origin = Vec3(0,0,0);
    }
/*    Vec3 origin = Vec3(0,0,0);
    Vec3 top_left_corner = Vec3(-2,1,-1);
    Vec3 horizontal = Vec3(4,0,0);
    Vec3 vertical = Vec3(0,-2,0); */
    Vec3 origin;
    Vec3 top_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    ray get_ray(float u, float v){return {origin, top_left_corner + u * horizontal + v * vertical};};

};

#endif //PATHTRACER_CAMERA_H
