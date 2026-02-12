#ifndef HITABLE_H_
#define HITABLE_H_

#include "ray.h"

struct hit_record {
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    bool front_face;

    void set_face_normal(const ray& r, glm::vec3 outward_normal){
        if(glm::dot(r.direction(),outward_normal)<0){
            //ray hitting point from outside
            //point is in front side
            normal =outward_normal;

        }else{
            //ray hitting point from inside
            //point is in back side
            normal = -outward_normal;
        }
    }
};

class hitable{
    public:
        virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const=0;
};

#endif