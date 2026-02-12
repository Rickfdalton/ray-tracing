#ifndef HITABLELIST_H_
#define HITABLELIST_H_

#include "hitable.h"

/*
class that stores a list of hittables.
*/

class hitable_list : public hitable
{
public:
    hitable_list(){}
    hitable_list(hitable** l, int n):list(l), list_size(n){}
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override{
        bool hit_anything = false;
        hit_record temp;
        float closest = t_max;

        for (int i=0; i<list_size; i++){
            if(list[i]->hit(r,t_min,closest,temp)){
                hit_anything= true;
                closest=temp.t;
                rec= temp;
            }

        }
        return hit_anything;

    }
    hitable** list;
    int list_size;

};


#endif