//
// Created by Thierry Backes on 21/10/18.
//

#ifndef PATHTRACER_MATH_HELPER_H
#define PATHTRACER_MATH_HELPER_H

#include "vec3.h"

/**
 * Normalizes value in given min/max range
 * https://stats.stackexchange.com/questions/70801/how-to-normalize-data-to-0-1-range
 */
float normalize(float in,float min, float max){
    return (in-min)/(max-min);
}

/**
 * Returns a random Point on the unit sphere
 * http://mathworld.wolfram.com/SpherePointPicking.html
 */
Vec3 random_point_on_unit_sphere(){
    auto phi = static_cast<float>(drand48() * 2 * M_PI);
    auto u = static_cast<float>(drand48() * 2 - 1);
    float t = sqrtf(1-u*u);
    return {t*cosf(phi),t*sinf(phi),u};
}

/**
 * Returns a random point from the unit disk
 * http://mathworld.wolfram.com/DiskPointPicking.html
 */
Vec3 random_point_on_unit_disk(){
    auto theta = static_cast<float>(drand48() * 2 * M_PI);
    auto r = static_cast<float>(drand48());
    return {sqrtf(r)*cosf(theta),sqrtf(r)*sinf(theta),0};
}


#endif //PATHTRACER_MATH_HELPER_H
