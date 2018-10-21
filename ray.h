//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_RAY_H
#define PATHTRACER_RAY_H
#include "vec3.h"

class ray{
public:
    ray() = default;
    ray(const Vec3 ori, const Vec3 dir):origin(ori){
        //direction = normalized(dir);
        direction = (dir);
    };

    const Vec3 &getOrigin() const {
        return origin;
    }

    const Vec3 &getDirection() const {
        return direction;
    }

    Vec3 pointAtParam(float t) const{return origin+t*direction;}

private:
    Vec3 origin;
    Vec3 direction;
};

#endif //PATHTRACER_RAY_H