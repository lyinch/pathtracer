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

    bool bounding_box(float t0, float t1, aabb &box) override;

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


/**
 * We combone the bounding boxes from the two timepoints t0 and t1
 * @param t0
 * @param t1
 * @param box
 * @return
 */
bool moving_sphere::bounding_box(float t0, float t1, aabb &box) {
    aabb box0 = aabb(center(t0)-Vec3(radius,radius,radius),center(t0)+Vec3(radius,radius,radius));
    aabb box1 = aabb(center(t1)-Vec3(radius,radius,radius),center(t1)+Vec3(radius,radius,radius));
    //box = box1 = aabb(Vec3(10,10,10),Vec3(11,11,11));
    box = surrounding_box(box0,box1);
    return true;
}

#endif //PATHTRACER_MOVING_SPHERE_H
