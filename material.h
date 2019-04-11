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


/**
 * Lambertian shading is independent of the view direction. We use Lambert's cosine law to calculate the amount
 * of light arriving at the surface. Diffuse shading is the uniform scattering in all directions. Ideal diffuse
 * reflection is Lambertian reflection.
 * The albedo specifies the ratio of reflected light over the amount of incident light
 */
class lambertian: public material{
public:
    lambertian(const Vec3& a):albedo(a){};

    bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const override {
        // diffuse: uniform scattering in all directions on the unit hemisphere
        Vec3 target = rec.p + rec.normal + random_point_on_unit_sphere();
        // cosine law
        scattered = ray(rec.p,target-rec.p,r_in.time());
        attenuation = albedo;
        return true;
    }
    Vec3 albedo;

};

/**
 * Gives the perfect reflection ray
 * @param v The in vector
 * @param n The surface normal
 * @return perfect reflection ray
 */
Vec3 reflect(const Vec3 &v, const Vec3 &n){
    return v-2*dot(v,n)*n;
}

/**
 * Approximation of the fresnel factor for specular reflection.
 * @param cosine
 * @param ref_idx
 * @return
 */
float schlick(float cosine, float ref_idx){
    float r0 = (1-ref_idx)/(1+ref_idx);
    r0 = r0*r0;
    return static_cast<float>(r0 + (1 - r0) * pow((1 - cosine), 5));
}


/**
 * Checks if an incoming vector v is being refracted or has a total inner reflection. A bit weird code, this apparently
 * implements snell's law
 * @param v Incoming ray
 * @param n Normal vector
 * @param ni_over_nt Refraction indices of media
 * @param refracted Pass by reference return value of the refracted ray
 * @return boolean whether a refraction happens
 */
bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted){
    Vec3 uv = normalized(v);
    float dt = dot(uv,n);
    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1-dt*dt);
    // Check for total inner reflection
    if(discriminant > 0){
        refracted = ni_over_nt*(uv-n*dt)-n*sqrtf(discriminant);
        return true;
    }
    return false;
}


/**
 * Clear materials are dielectrics (water, glass,...) We have a reflection and refraction part. Snell's law
 * describes the refraction part, i.e. when the ray travels from one medium to another.
 */
class dielectric : public material{
public:
    explicit dielectric(float ri):ref_idx(ri){};

    bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const override {
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

        // The probability of a reflection/refraction depends on the angle we're looking at the surface
        if(refract(r_in.getDirection(),outward_normal,ni_over_nt,refracted)){
            reflect_prob = schlick(cosine,ref_idx);
        }else{
            reflect_prob = 1.0f;
        }

        // randomly chose between reflection and refraction
        if(drand48() < reflect_prob)
            scattered = ray(rec.p,reflected,r_in.time());
        else
            scattered = ray(rec.p,refracted,r_in.time());

        return true;
    }

    float ref_idx;
};

/**
 * Smooth metals are perfect mirrors. A fuzz factor gives us "unclean" metal. Note that this metal
 * class does not have specular highlights!
 */
class metal: public material{
public:
    metal(const Vec3& albedo, float fuzz): albedo(albedo) {this->fuzz = fuzz < 1 ? fuzz : 1;};

    bool scatter(const ray& r_in, const hit_record& rec, Vec3& attenuation, ray& scattered) const override {
        Vec3 reflected = reflect(r_in.getDirection(),rec.normal);

        scattered = ray(rec.p,reflected + fuzz*random_point_on_unit_sphere(),r_in.time());
        attenuation = albedo;
        return(dot(scattered.getDirection(),rec.normal) > 0);
    }
    float fuzz;
    Vec3 albedo;
};

#endif //PATHTRACER_MATERIAL_H
