#ifndef SPHERE_H_
#define SPHERE_H_
#include "hitable.h"

/*
this class implements the abstract class hitable
*/
class sphere: public hitable
{
public:
    sphere() {};
    sphere(glm::vec3 center, float radius, material* material_ptr): center(center), radius(radius),material_ptr(material_ptr) {};
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override{
        /*
        for those points present in ray and in the sphere.
        t*t*dot(B, B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
        */
        glm::vec3 oc = r.origin() - center;
        float a = glm::dot(r.direction(),r.direction());
        float b = 2* glm::dot(r.direction(),oc);
        float c = glm::dot(oc,oc) - radius * radius;
        float discriminant = b*b - 4*a*c;
        if (discriminant>0){
            float temp = (-b - sqrt(discriminant))/(2.0*a);
            if (temp > t_min && temp < t_max){
                rec.t = temp;
                rec.p = r.point_at_param(temp);
                glm::vec3 out_normal = (rec.p - center)/radius;
                rec.set_face_normal(r,out_normal);
                rec.mat_ptr = material_ptr;
                return true;
            }
            temp = (-b + sqrt(discriminant))/(2.0*a);
            if (temp > t_min && temp < t_max){
                rec.t = temp;
                rec.p = r.point_at_param(temp);
                glm::vec3 out_normal = (rec.p - center)/radius;
                rec.set_face_normal(r,out_normal);
                rec.mat_ptr = material_ptr;

                return true;
            }
        }
        return false;

    }
    glm::vec3 center;
    float radius;
    material* material_ptr;
};


#endif