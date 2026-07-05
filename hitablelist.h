#ifndef HITABLELIST_H_
#define HITABLELIST_H_

#include <memory>
#include <vector>
#include "hitable.h"

/*
class that stores a list of hittables.
*/

class hitable_list : public hitable
{
public:
    hitable_list(){}
    hitable_list(std::shared_ptr<hitable> object){ add(object); }

    void add(std::shared_ptr<hitable> object){
        list.push_back(object);
        bbox = list.size() == 1 ? object->bounding_box() : aabb(bbox, object->bounding_box());
    }

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override{
        bool hit_anything = false;
        hit_record temp;
        float closest = t_max;

        for (const auto& object : list){
            if(object->hit(r,t_min,closest,temp)){
                hit_anything= true;
                closest=temp.t;
                rec= temp;
            }

        }
        return hit_anything;

    }

    aabb bounding_box() const override{
        return bbox;
    }

private:
    std::vector<std::shared_ptr<hitable>> list;
    aabb bbox;


};


#endif
