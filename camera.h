//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "ray.h"
#include "math_helper.h"

class camera{
public:
    camera(Vec3 lookfrom, Vec3 lookat) : camera(lookfrom,lookat,Vec3(0,1,0)){}; //constructor if we only provide origin and lookat
    camera(Vec3 lookfrom = Vec3(0,0,0), Vec3 lookat = Vec3(0,0,-1), Vec3 vup = Vec3(0,1,0), float vfov = 90, float aspect = 2.0f, float aperture = 0, float focus_dist = 1){
        Vec3 u,v,w;
        lens_radius = aperture/2;
        auto theta = static_cast<float>(vfov * M_PI / 180.0f);
        auto half_height = tanf(theta / 2);
        float half_width = aspect*half_height;
        w = normalized(lookfrom-lookat);
        u = normalized(cross(vup,w));
        v = cross(w,u);
        top_left_corner = origin-focus_dist*half_width*u+half_height*v-focus_dist*w;
        horizontal = 2*focus_dist*half_width*u;
        vertical = -2*focus_dist*half_height*v;
        origin = lookfrom;
    }

    Vec3 origin;
    Vec3 top_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    float lens_radius;

    ray get_ray(float u, float v){
            Vec3 rd = lens_radius*random_point_on_unit_disk();
            Vec3 offset = Vec3(u*rd.x,v*rd.y,0);
        return {origin+offset, top_left_corner + u * horizontal + v * vertical-origin-offset};
    };

};

#endif //PATHTRACER_CAMERA_H
