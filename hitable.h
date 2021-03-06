//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_HITABLE_H
#define PATHTRACER_HITABLE_H
#include "ray.h"
#include "aabb.h"
#include <memory>

class material;

struct hit_record{
    float t{}; //z coordinate of hit
    Vec3 p; //exact point of hit
    Vec3 normal;
    std::shared_ptr<material> mat_ptr;
};

class hitable{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) = 0;
};
#endif //PATHTRACER_HITABLE_H
