#ifndef RAY_H_
#define RAY_H_
#include <glm/glm.hpp>

/*
P_t = t*B +A;
*/
class ray{
public:
    ray(){};
    ray(const glm::vec3 &a, const glm::vec3 &b, double time) : orig(a) , dir(b), tm(time){}
    ray(const glm::vec3 &a, const glm::vec3 &b) : ray(a,b,0){}
    glm::vec3 origin() const{return orig;}
    glm::vec3 direction() const {return dir;}
    glm::vec3 point_at_param(float t )const{return orig+t*dir;}
    double time() const{return tm;}

    glm::vec3 orig;
    glm::vec3 dir;
    double tm;
};

#endif

