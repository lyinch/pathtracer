//
// Created by Thierry Backes on 20/10/18.
//

#ifndef PATHTRACER_HITABLE_LIST_H
#define PATHTRACER_HITABLE_LIST_H
#include <cstdio>
#include <iostream>
#include "hitable.h"
class hitable_list: public hitable{
public:
    hitable_list() = default;
    hitable_list(hitable **l, int n):list_size(n),list(l){};
    int list_size;
    hitable **list;

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
};

bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {

    hit_record tmp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for(int i = 0; i < list_size; ++i){
        if(list[i]->hit(r,t_min,closest_so_far,tmp_rec)){

            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}
#endif //PATHTRACER_HITABLE_LIST_H
