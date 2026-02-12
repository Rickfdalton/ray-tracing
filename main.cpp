/*
ray tracing
*/
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "ray.h"
#include "sphere.h"
#include "hitablelist.h"



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
    outFile << "P3\n" << nx << " " <<ny << "\n255\n";
    glm::vec3 lower_left_corner(-2.0,-1.0,-1.0);
    glm::vec3 horizontal(4.0,0.0,0.0);
    glm::vec3 vertical(0.0,2.0,0.0);
    glm::vec3 origin(0.0,0.0,0.0);

    hitable* list[2];
    list[0]= new sphere(glm::vec3(0,0,-1),0.5);
    list[1]= new sphere(glm::vec3(0,-100.5,-1),100);
    hitable* world = new hitable_list(list,2);

    for (int j=ny-1 ; j >= 0; j--){
        for (int i=0; i< nx ; i++){
            float u = float(i)/float(nx);
            float v = float(j)/float(ny);
            glm::vec3 direction = lower_left_corner + u*horizontal + v*vertical;
            ray r(origin,direction);
            glm::vec3 col = color(r,world);
            int ir = static_cast<int>(255 * col[0]);
            int ig = static_cast<int>(255 * col[1]);
            int ib = static_cast<int>(255 * col[2]);
            outFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    outFile.close();
}