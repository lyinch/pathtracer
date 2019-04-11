#include <utility>

//
// Created by Thierry Backes on 11/04/19.
//

#ifndef PATHTRACER_MOVING_SPHERE_H
#define PATHTRACER_MOVING_SPHERE_H


#include "hitable.h"


class moving_sphere: public hitable {
public:
    moving_sphere() = default;
    moving_sphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, std::shared_ptr<material> m):center0(cen0),center1(cen1),time0(t0),time1(t1),radius(r), mat(
            std::move(m)){};
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;
    Vec3 center (float time) const;
    Vec3 center0;
    Vec3 center1;
    float time0;
    float time1;
    float radius;
    std::shared_ptr<material> mat;
};

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    Vec3 originToCenter = r.getOrigin()-center(r.time());
    float a = dot(r.getDirection(),r.getDirection());
    float b = 2.f*dot(originToCenter,r.getDirection());
    float c = dot(originToCenter,originToCenter)-radius*radius;
    float discriminant = b*b-4.f*a*c;

    if(discriminant > 0) {
        float intersection = (-b-sqrtf(discriminant))/(2.0f*a);

        if(intersection < t_max && intersection > t_min){

            rec.t = intersection;
            rec.p = r.pointAtParam(intersection);
            rec.normal = normalized((rec.p-center(r.time()))/radius);
            rec.mat_ptr = mat;
            return true;
        }

        intersection = (-b+sqrtf(discriminant))/(2.0f*a);
        if(intersection < t_max && intersection > t_min){
            rec.t = intersection;
            rec.p = r.pointAtParam(intersection);
            rec.normal = normalized((rec.p-center(r.time()))/radius);
            rec.mat_ptr = mat;
            return true;
        }

    }

    return false;
}

Vec3 moving_sphere::center(float time) const {
    return center0 + ((time-time0)/(time1-time0))*(center1-center0);
}

#endif //PATHTRACER_MOVING_SPHERE_H
