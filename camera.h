//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "ray.h"

class camera{
public:
    camera() = default;
    Vec3 origin = Vec3(0,0,0);
    Vec3 top_left_corner = Vec3(-2,1,-1);
    Vec3 horizontal = Vec3(4,0,0);
    Vec3 vertical = Vec3(0,-2,0);

    ray get_ray(float u, float v){return ray(origin, top_left_corner + u * horizontal + v * vertical);};

};

#endif //PATHTRACER_CAMERA_H
