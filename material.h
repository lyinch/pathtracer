//
// Created by Thierry Backes on 21/10/18.
//

#ifndef PATHTRACER_MATERIAL_H
#define PATHTRACER_MATERIAL_H

#include "ray.h"
#include "hitable.h"
#include "math_helper.h"

class material{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const = 0;
};

class lambertian: public material{
public:
    lambertian(const Vec3& a):albedo(a){};

    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const{
        Vec3 target = rec.p + rec.normal + random_point_on_unit_sphere();
        scattered = ray(rec.p,target-rec.p);
        attenuation = albedo;
        return true;
    }
    Vec3 albedo;

};

class metal: public material{
public:
    metal(const Vec3& a): albedo(a){};

    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) {

        return false;
    }
    Vec3 albedo;
};

#endif //PATHTRACER_MATERIAL_H
