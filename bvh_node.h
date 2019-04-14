//
// Created by Thierry Backes on 12/04/19.
//

#ifndef PATHTRACER_BVH_NODE_H
#define PATHTRACER_BVH_NODE_H


#include <iostream>
#include <vector>
#include "hitable.h"

class bvh_node: public hitable{
public:
    bvh_node(){}
    bvh_node(std::vector<std::shared_ptr<hitable>>  list,int l, int r, float t0, float t1);

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;
    bool bounding_box(float t0, float t1, aabb &box) override;
    std::shared_ptr<hitable> left;
    std::shared_ptr<hitable> right;
    aabb box;
};

bool bvh_node::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    if(box.hit(r,t_min,t_max)){
        hit_record left_rec;
        hit_record right_rec;
        bool hit_left = left->hit(r,t_min,t_max,left_rec);
        bool hit_right = right->hit(r,t_min,t_max,right_rec);
        if(hit_left && hit_right){
            if(left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            return true;
        }else if(hit_left){
            rec = left_rec;
            return true;
        }else if(hit_right){
            rec = right_rec;
            return true;
        }
    }
    return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &box) {
    box = this->box;
    return true;
}

bool box_x_compare(const std::shared_ptr<hitable> &a,const std::shared_ptr<hitable> &b){
    aabb box_left, box_right;

    hitable * ah = a.get();
    hitable * bh = b.get();
    if(!ah->bounding_box(0,0,box_left) || !bh->bounding_box(0,0,box_right))
        std::cerr << "No bounding box in bvh node constructor\n";
    return box_left.min().x - box_right.min().x >= 0.0;
}

bool box_y_compare(const std::shared_ptr<hitable> &a,const std::shared_ptr<hitable> &b){
    aabb box_left, box_right;

    hitable * ah = a.get();
    hitable * bh = b.get();
    if(!ah->bounding_box(0,0,box_left) || !bh->bounding_box(0,0,box_right))
        std::cerr << "No bounding box in bvh node constructor\n";
    return box_left.min().y - box_right.min().y >= 0.0;
}

bool box_z_compare(const std::shared_ptr<hitable> &a,const std::shared_ptr<hitable> &b){
    aabb box_left, box_right;

    hitable * ah = a.get();
    hitable * bh = b.get();
    if(!ah->bounding_box(0,0,box_left) || !bh->bounding_box(0,0,box_right))
        std::cerr << "No bounding box in bvh node constructor\n";
    return box_left.min().z - box_right.min().z >= 0.0;
}

bvh_node::bvh_node(std::vector<std::shared_ptr<hitable>>  list, int l,int r, float t0, float t1) {
    std::cout << "BVH NODE CONSTRUCTOR CALLED " << l << "   " << r <<  std::endl;
    int axis = int(3*drand48()); //randomly choose an axis
    if(axis == 0)
        std::sort(list.begin()+l,list.begin()+r,box_x_compare);
    else if(axis == 1)
        std::sort(list.begin()+l,list.begin()+r,box_y_compare);
    else
        std::sort(list.begin()+l,list.begin()+r,box_z_compare);

    if(r-l == 1){
        left = right = list.at(l);
    }else if(r-l == 2){
        left = list.at(l);
        right = list.at(l+1);
    }
    else if(r-l> 2){
        left = std::shared_ptr<hitable>(new bvh_node(list,l,r/2,t0,t1));
        right = std::shared_ptr<hitable>(new bvh_node(list,l+r/2,r,t0,t1));
        //left = std::shared_ptr<hitable>(new bvh_node(l,n/2,t0,t1));
        //right = std::shared_ptr<hitable>(new bvh_node(l+n/2,n-n/2,t0,t1));
    }
    aabb box_left, box_right;
    if(!left->bounding_box(t0,t1,box_left) || !right->bounding_box(t0,t1,box_right))
        std::cerr << "No bounding box in bvh node constructor\n";
    box = surrounding_box(box_left,box_right);

}



#endif //PATHTRACER_BVH_NODE_H
