//
// Created by Thierry Backes on 24/10/18.
//

#ifndef PATHTRACER_AABB_H
#define PATHTRACER_AABB_H

#include "vec3.h"
#include "ray.h"

class aabb{
public:
    aabb(){};
    aabb(const Vec3 &a, const Vec3 &b):_min(a),_max(b) {}
    Vec3 min() const {return _min;}
    Vec3 max() const {return _max;}
    // For a ray p(t) = A + tB and a bounding box with bounds x=_min.x , y = min.y etc.
    // the hitpoints t0 and t1 can be calculated for each plate separately
    bool hit(const ray &r, float tmin, float tmax) const{

        for (int a = 0; a < 3; ++a) {
            float t0 = fminf((_min[a] - r.getOrigin()[a])/r.getDirection()[a],
                             (_max[a]-r.getOrigin()[a])/r.getDirection()[a]);
            float t1 = fmaxf((_min[a] - r.getOrigin()[a])/r.getDirection()[a],
                             (_max[a]-r.getOrigin()[a]/r.getDirection()[a]));
            tmin = fmaxf(t0,tmin);
            tmax = fminf(t1,tmax);
            if(tmax <= tmin)
                return false;
        }


        return true;
    }

    Vec3 _min;
    Vec3 _max;


};

std::ostream& operator<< (std::ostream &out, aabb const& data) {
    out << "min: (" <<  data._min.x <<  ',' << data._min.y << "," << data._min.z << ") ";
    out << "max: (" <<  data._max.x <<  ',' << data._max.y << "," << data._max.z << ") ";
    return out;
}

aabb surrounding_box(aabb box0, aabb box1){
    Vec3 small (fminf(box0.min().x,box1.min().x),
                fminf(box0.min().y,box1.min().y),
                fminf(box0.min().z,box1.min().z));
    Vec3 big (fmaxf(box0.max().x,box1.max().x),
              fmaxf(box0.max().y,box1.max().y),
              fmaxf(box0.max().z,box1.max().z));

    return aabb(small,big);
}
#endif //PATHTRACER_AABB_H
