#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hitable.h"
#include "hitablelist.h"
#include <algorithm>

class bvh_node: public hitable {
    public:
        bvh_node(hitable_list list) : bvh_node(list.list, 0, list.list.size()){

        }

        bvh_node(std::vector<std::shared_ptr<hitable>>&objects, size_t start, size_t end){
            // this method to construct bvh.
            // we call from the large aabb and then recursively to small aabb

            for (size_t i = start; i < end; i++){
                bbox = aabb(bbox, objects[i]->bounding_box());
            }

            double x_len = bbox.x.max - bbox.x.min;
            double y_len = bbox.y.max - bbox.y.min;
            double z_len = bbox.z.max - bbox.z.min;

            int axis = 0;
            if (y_len > x_len && y_len > z_len) axis = 1;
            else if (z_len > x_len && z_len > y_len) axis = 2;

            auto box_compare = [axis](const std::shared_ptr<hitable>&a, const std::shared_ptr<hitable>&b){
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
            // bbox= aabb(left->bounding_box(),right->bounding_box());
        }

        bool hit(const ray&r, float t_min, float t_max, hit_record& rec) const override{
            if(!bbox.hit(r, interval(t_min, t_max))) return false;
            bool hit_left = left->hit(r,t_min,t_max,rec);
            bool hit_right = right->hit(r,t_min, hit_left ? rec.t: t_max,rec);
            return hit_left || hit_right;
        }

        aabb bounding_box() const override {return bbox;}

    private:
        std::shared_ptr<hitable> left;
        std::shared_ptr<hitable> right;
        aabb bbox;
};

#endif