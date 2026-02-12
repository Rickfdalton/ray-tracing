/*
ray tracing
*/
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "ray.h"
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"



glm::vec3 color(const ray& r, hitable* world){
    hit_record rec;
    if (world->hit(r,0.0,MAXFLOAT,rec)){
        return 0.5f*glm::vec3(rec.normal.x+1, rec.normal.y+1,rec.normal.z+1);
    }

    glm::vec3 dir = r.direction();
    glm::vec3 unit_dir = glm::normalize(dir);
    float t = 0.5f * (unit_dir.y + 1.0f);
    return (1.0f - t)*(glm::vec3(1.0,1.0,1.0)) + t*(glm::vec3(0.5,0.7,1.0));
}




int main(){
    std::ofstream outFile("hittable.ppm", std::ios::out);

    int nx = 400;
    int ny = 200;
    int ns = 100;
    outFile << "P3\n" << nx << " " <<ny << "\n255\n";


    hitable* list[3];
    list[0]= new sphere(glm::vec3(0,0,-1),0.5);
    list[1]= new sphere(glm::vec3(1,0,-1),0.5);
    list[2]= new sphere(glm::vec3(0,-100.5,-1),100);
    hitable* world = new hitable_list(list,3);

    camera cam;

    for (int j=ny-1 ; j >= 0; j--){
        for (int i=0; i< nx ; i++){
     
            glm::vec3 col(0.0,0.0,0.0);
            for (int s=0;s<ns;s++){
                float u = float(i+drand48())/float(nx);
                float v = float(j+drand48())/float(ny);
                ray r = cam.get_ray(u,v);
                col+=color(r,world);
            }
            col/=float(ns);
      
            int ir = static_cast<int>(255 * col[0]);
            int ig = static_cast<int>(255 * col[1]);
            int ib = static_cast<int>(255 * col[2]);
            outFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    outFile.close();
}