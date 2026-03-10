#ifndef CAMERA_H_
#define CAMERA_H_
#include <cmath>
#include "ray.h"

class camera{
public:
    camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 world_up,float vertical_fov , float aspect_ratio){ // vertical_pov is in degree, look_at is a point
        float theta = vertical_fov * M_PI /180.0f;
        float tan_theta_by_2 = tan(theta/2.0f);
        float height = 2*tan_theta_by_2;
        float width = aspect_ratio*height;

        origin = lookfrom;
        glm::vec3 lookat_vec= glm::normalize(lookat-lookfrom);
        glm::vec3 cam_right = glm::normalize(glm::cross( lookat_vec, world_up));
        glm::vec3 cam_up = glm::normalize(glm::cross(cam_right,lookat_vec));

        horizontal= width * cam_right;
        vertical= height * cam_up;
        lower_left_corner = origin + lookat_vec- 0.5f *vertical - 0.5f*horizontal;
    }

    ray get_ray(float u, float v){
        glm::vec3 direction = lower_left_corner + u*horizontal + v*vertical - origin;
        return ray(origin,direction);
    }

    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 origin;
    glm::vec3 direction;


};

#endif