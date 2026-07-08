#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hitable.h"
#include "hitablelist.h"
#include <algorithm>

class bvh_node: public hitable {
    public:
        bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()){

        }

        bvh_node(std::vector<shared_ptr<hitable>>&objects, size_t start, size_t end){
            // this method to construct bvh.
            // we call from the large aabb and then recursively to small aabb
            int axis = rand()%3;

            auto box_compare = [axis](const shared_ptr<hitable>&a, const shared_ptr<hitable>&b){
                return a->bounding_box().axis_interval(axis).min < b->bounding_box().axis_interval(axis).min;
            };// we need lambda function coz comparator needs 2 parameters.


            size_t object_span = end - start;

            if (object_span ==1){
                left=right=objects[start];
            }else if (object_span ==2){
                left=objects[start];
                right=objects[start+1];
            }else{
                std::sort(objects.begin()+start, objects.begin()+end, box_compare);
                auto mid = start+ object_span/2;
                left = std::make_shared<bvh_node>(objects, start, mid);
                right = std::make_shared<bvh_node>(objects, mid, end);
            }
            bbox= aabb(left->bounding_box(),right->bounding_box());
        }

        bool hit(const ray&r, interval ray_t, hit_record& rec) const override{
            if(!bbox.hit(r, ray_t)) return false;
            bool hit_left = left->hit(r,ray_t,rec);
            bool hit_right = right->hit(r,interval(ray_t.min, hit_left ? rec.t: ray_t.max),rec);
            return hit_left || hit_right;
        }

        aabb bounding_box() const override {return bbox;}

    private:
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
        aabb bbox;
};

#endif