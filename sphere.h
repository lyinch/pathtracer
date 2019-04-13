#include <utility>

//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_SPHERE_H
#define PATHTRACER_SPHERE_H
#include <iostream>
#include "hitable.h"
#include <memory>

class sphere: public hitable{
public:
    sphere() = default;
    sphere(Vec3 cen, float r, std::shared_ptr<material> m):center(cen),radius(r), mat(std::move(m)){};
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;

    bool bounding_box(float t0, float t1, aabb &box) override;

    Vec3 center;
    float radius;
    std::shared_ptr<material> mat;
};


bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    Vec3 originToCenter = r.getOrigin()-center;
    float a = dot(r.getDirection(),r.getDirection());
    float b = 2.f*dot(originToCenter,r.getDirection());
    float c = dot(originToCenter,originToCenter)-radius*radius;
    float discriminant = b*b-4.f*a*c;
    //std::cout << discriminant << std::endl;

    if(discriminant > 0) {
        float intersection = (-b-sqrtf(discriminant))/(2.0f*a);

        if(intersection < t_max && intersection > t_min){

            rec.t = intersection;
            rec.p = r.pointAtParam(intersection);
            rec.normal = normalized((rec.p-center)/radius);
            rec.mat_ptr = mat;
            return true;
        }

        intersection = (-b+sqrtf(discriminant))/(2.0f*a);
        if(intersection < t_max && intersection > t_min){
            rec.t = intersection;
            rec.p = r.pointAtParam(intersection);
            rec.normal = normalized((rec.p-center)/radius);
            rec.mat_ptr = mat;
            return true;
        }

    }

    return false;
}

std::ostream& operator<< (std::ostream &out, sphere const& data) {
    out << "Sphere: (" <<  data.center.x <<  ',' << data.center.y << "," << data.center.z << "," << data.radius << ") ";
    return out;
}

bool sphere::bounding_box(float t0, float t1, aabb &box) {
    box = aabb(center-Vec3(radius,radius,radius),center+Vec3(radius,radius,radius));

    std::cout << *this << " " <<  box << std::endl;
    return true;
}

#endif //PATHTRACER_SPHERE_H
