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

Vec3 reflect(const Vec3 &v, const Vec3 &n){
    return v-2*dot(v,n)*n;
}

float schlick(float cosine, float ref_idx){
    float r0 = (1-ref_idx)/(1+ref_idx);
    r0 = r0*r0;
    return static_cast<float>(r0 + (1 - r0) * pow((1 - cosine), 5));
}

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted){
    Vec3 uv = normalized(v);
    float dt = dot(uv,n);
    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1-dt*dt);
    if(discriminant > 0){
        refracted = ni_over_nt*(uv-n*dt)-n*sqrtf(discriminant);
        return true;
    }
    return false;
}

class dielectric : public material{
public:
    explicit dielectric(float ri):ref_idx(ri){};

    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const {
        Vec3 outward_normal;
        Vec3 reflected = reflect(r_in.getDirection(),rec.normal);
        float ni_over_nt;
        attenuation = Vec3(1.f,1.f,1.f);
        Vec3 refracted;
        float cosine;
        float reflect_prob;
        if(dot(r_in.getDirection(),rec.normal) > 0){
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.getDirection(),rec.normal)/r_in.getDirection().length();
        }else{
            outward_normal = rec.normal;
            ni_over_nt = 1.0f/ref_idx;
            cosine = -dot(r_in.getDirection(),rec.normal)/r_in.getDirection().length();
        }

        if(refract(r_in.getDirection(),outward_normal,ni_over_nt,refracted)){
            reflect_prob = schlick(cosine,ref_idx);
        }else{
            reflect_prob = 1.0f;
        }

        if(drand48() < reflect_prob)
            scattered = ray(rec.p,reflected);
        else
            scattered = ray(rec.p,refracted);

        return true;
    }

    float ref_idx;
};

class metal: public material{
public:
    metal(const Vec3& a, float f): albedo(a){fuzz = f < 1 ? f : 1;};

    virtual bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const{
        Vec3 reflected = reflect(r_in.getDirection(),rec.normal);
        scattered = ray(rec.p,reflected + fuzz*random_point_on_unit_sphere());
        attenuation = albedo;
        return(dot(scattered.getDirection(),rec.normal) > 0);
    }
    float fuzz;
    Vec3 albedo;
};

#endif //PATHTRACER_MATERIAL_H
