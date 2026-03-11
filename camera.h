#ifndef CAMERA_H_
#define CAMERA_H_
#include <cmath>
#include "ray.h"

class camera{
public:
    camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 world_up,float vertical_fov , float aspect_ratio, float focal_distance, float aperture){ // vertical_pov is in degree, look_at is a point
        float theta = vertical_fov * M_PI /180.0f;
        float tan_theta_by_2 = tan(theta/2.0f);
        float height = 2*tan_theta_by_2;
        float width = aspect_ratio*height;
        lens_radius = aperture / 2.0f;

        origin = lookfrom;
        glm::vec3 lookat_vec= glm::normalize(lookat-lookfrom);
        cam_right = glm::normalize(glm::cross( lookat_vec, world_up));
        cam_up = glm::normalize(glm::cross(cam_right,lookat_vec));

        horizontal= width * cam_right * focal_distance;
        vertical= height * cam_up * focal_distance;
        lower_left_corner = origin + focal_distance* lookat_vec- 0.5f *vertical - 0.5f*horizontal;
    }

    ray get_ray(float u, float v){
        //pick random point on disk
         glm::vec3 rd ;
         do{
            rd = origin + (2.0f*float(drand48())-1.0f)*lens_radius*cam_right + (2.0f*float(drand48())-1.0f)*lens_radius*cam_up;
         }while(glm::length(rd-origin) > lens_radius);

         glm::vec3 direction = lower_left_corner + u*horizontal + v*vertical - rd;
        return ray(rd,direction);
    }

    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec3 lookfrom;
    glm::vec3 cam_right;
    glm::vec3 cam_up;
    float lens_radius;

};

#endif