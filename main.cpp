/*
ray tracing
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include "ray.h"
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"


//get random point in a sphere with O as origin
glm::vec3 get_random_in_unit_sphere(){
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(drand48(),drand48(),drand48()) - glm::vec3(1.0,1.0,1.0);
    }while(glm::length(p) >= 1);
    return p;
}

//get the reflected vector
glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n){
    return v + 2.0f *(glm::dot(-v,n)*n);
}

//get the refracted vector
//using snells law
// n_i/n_o = sin(i)/sin(o)

/*
idea : this implemetation is not ray decomposition, this is geometry
this result in numerical instability and black spots
*/
bool refract_geo(const glm::vec3& v, const glm::vec3& n, float ni_over_no, glm::vec3& refracted) {
    glm::vec3 unit_v = glm::normalize(v);
    float cos_i = glm::dot(-unit_v, n);
    float sin_i = sqrt(1.0f - cos_i * cos_i);
    float sin_o = sin_i * ni_over_no;

    if (sin_o >= 1.0f)
        return false;  // TIR

    if (sin_o < 1e-2f){
        refracted = unit_v;
        return true;
    }

    float cos_o = sqrt(1.0f - sin_o * sin_o);
    float scale = (sin_i * cos_o - cos_i * sin_o) / sin_o;  // sin(i-o)/sin(o)
    refracted = unit_v + scale * (-n);
    return true;
}

/*
now we decompose incident ray
https://ocw.mit.edu/courses/6-837-computer-graphics-fall-2012/resources/mit6_837f12_lec13/
*/
bool refract_mit(const glm::vec3& v, const glm::vec3& n, float ni_over_no, glm::vec3& refracted) {
    glm::vec3 unit_v = glm::normalize(v);
    float cos_i = glm::dot(-unit_v, n);
    float k = 1.0f - ni_over_no*ni_over_no*(1.0f - cos_i*cos_i);
    if (k<0){
        return false;
    }
    refracted = (unit_v+ (glm::dot(-unit_v,n))*n)*ni_over_no -n*(sqrt(k));
    return true;
}

/*schlick approximation
in glass materials some rays do reflect at the surface. we should model this.
when the incident angle increases, the probability of reflecting increases.*/
float schlick(float cosine, float ref_idx){
    float r0 = (1-ref_idx)/(1+ref_idx);
    r0 = r0 * r0;
    return r0 + (1-r0)*pow((1-cosine),5);
}


class material{
public:
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        glm::vec3& attenuation,
        ray& r_scattered
    ) const =0;
    virtual ~material() {}
};

class lambertian : public material{
public:
    lambertian(const glm::vec3& a) : albedo(a){}
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        glm::vec3& attenuation,
        ray& r_scattered
    ) const override {
        glm::vec3 target = rec.p + rec.normal + get_random_in_unit_sphere();
        r_scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }

    glm::vec3 albedo; //reflectivity
};

//metal reflects using the reflect formula not just scatters
class metal : public material{
public:
    metal(const glm::vec3& a):albedo(a){}
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        glm::vec3& attenuation,
        ray& r_scattered
    ) const override {
        r_scattered = ray(rec.p, reflect(glm::normalize(r_in.direction()), rec.normal));
        attenuation= albedo;
        return (glm::dot(r_scattered.direction(),rec.normal)>0);
    }

    glm::vec3 albedo;
};

//glass refracts
class glass : public material{
public:
    glass(float ref_idx): ref_idx(ref_idx){};
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        glm::vec3& attenuation,
        ray& r_scattered
    ) const override{
    attenuation = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 reflected = reflect(r_in.direction(),rec.normal);
    float ni_over_no = rec.front_face ? 1.0f /ref_idx : ref_idx  ;
    float cosine = dot(-glm::normalize(r_in.direction()),rec.normal); 
    float reflect_prob;
    // glm::vec3 outward_normal = rec.front_face ? rec.normal : -rec.normal ;
    glm::vec3 refracted;
    if(refract_mit(r_in.direction(),rec.normal, ni_over_no, refracted)){
        reflect_prob = schlick(cosine, ref_idx);
    }
    else{
        reflect_prob = 1.0f;
    }
    if (drand48() < reflect_prob){
        r_scattered = ray(rec.p, reflected );

    }else{
        r_scattered = ray(rec.p, refracted );
    }
    return true;
    }

    float ref_idx;
};

// depth? how many bounces are still allowed
glm::vec3 color(const ray& r, hitable* world , int depth){
    hit_record rec;
    if (world->hit(r,0.0001f,MAXFLOAT,rec)){
        //1. calculate diffuse shading : direct lighting, no re emitting

        /*glm::vec3 object_color(0.3f,0.9f,0.3f);
        float diffuse = glm::dot(rec.normal, -r.direction());
        if (diffuse<0){
            diffuse = 0.0f;
        }
        glm::vec3 diffuse_light = diffuse*object_color;
        return 0.5f* diffuse_light;*/

        //2. but this is not intersting, we are not going to reemit the light ray into several directions
        // monte carlo integration
        // we chose the hitpoint, draw an imaginary sphere touching the hitpoint with radius unit normal, get random point inside the sphere and send ray
        // glm::vec3 target = rec.p + rec.normal + get_random_in_unit_sphere();
        // return 0.4f * color(ray(rec.p, target-rec.p), world);

        //3. lambertian and metal impl
        ray r_scattered;
        glm::vec3 attenuation;

        if(depth <50 && rec.mat_ptr->scatter(r,rec,attenuation,r_scattered) ){
            return attenuation*color(r_scattered, world, depth+1) ;
        }else{
            return glm::vec3(0,0,0) ;
        }
        // return 0.5f*glm::vec3(rec.normal.x+1, rec.normal.y+1,rec.normal.z+1);
    }

    glm::vec3 dir = r.direction();
    glm::vec3 unit_dir = glm::normalize(dir);
    float t = 0.5f * (unit_dir.y + 1.0f);
    return (1.0f - t)*(glm::vec3(1.0,1.0,1.0)) + t*(glm::vec3(0.5,0.7,1.0));
}


hitable_list* scene(hitable** list, int& i){

    //earth or big sphere
    list[i++]= new sphere(glm::vec3(0,-1000.0,0),1000, std::shared_ptr<material>(new lambertian(glm::vec3(0.5,0.5,0.5))));
    for(int a= -11; a<11; a++){ // x axis spread
        for(int b=-11;b<11;b++){ // z axis spread
            glm::vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());// small spheres with 0.2 radius
            float choose_mat = drand48();

            if(glm::length(center- glm::vec3(4,0.2,0)) > 0.9){ // prevent small balls inside big balls

                if (choose_mat < 0.8) {
                    //create 80% of diffuse materials
                    list[i++] = new sphere(center, 0.2, std::shared_ptr<material> (new lambertian(glm::vec3(drand48()*drand48(),drand48()*drand48(),drand48()*drand48()))));//more darker as it absorb light
                }
                else if (choose_mat < 0.95)
                {
                    //create 15% metal
                    list[i++] = new sphere(center, 0.2, std::shared_ptr<material> (new metal(glm::vec3(0.5*(drand48()+1),0.5*(drand48()+1),0.5*(drand48()+1))))); // more brigther

                }else{
                    //5% glass
                    list[i++] = new sphere(center, 0.2, std::shared_ptr<material> (new glass(1.5f))); // more brigther
                }
                
            }
        }
    }
    list[i++] = new sphere(glm::vec3(0,1,0),1.0,std::shared_ptr<material> (new glass(1.5)));
    list[i++] = new sphere(glm::vec3(-4,1,0),1.0,std::shared_ptr<material> (new lambertian(glm::vec3(0.4,0.2,0.1))));
    list[i++] = new sphere(glm::vec3(4,1,0),1.0,std::shared_ptr<material> (new metal(glm::vec3(0.7,0.6,0.5))));
    return new hitable_list(list, i);
}

int main(){
    std::ofstream outFile("outputs/full_scene.ppm", std::ios::out);

    int nx = 600;
    int ny = 300;
    int ns = 200;
    outFile << "P3\n" << nx << " " <<ny << "\n255\n";

    // hitable* list[5];
    // list[0]= new sphere(glm::vec3(0,0,-1),0.5 , std::shared_ptr<material>(new lambertian(glm::vec3(0.8,0.3,0.3))));
    // list[1]= new sphere(glm::vec3(0,-100.5,-1),100, std::shared_ptr<material>(new lambertian(glm::vec3(0.8,0.8,0.0))));
    // list[2]= new sphere(glm::vec3(1,0,-1),0.5, std::shared_ptr<material>(new metal(glm::vec3(0.8,0.6,0.2))));
    // list[3]= new sphere(glm::vec3(-1,0,-1),0.5, std::shared_ptr<material>(new glass(1.5)));
    // list[4]= new sphere(glm::vec3(-1,0,-1),-0.45, std::shared_ptr<material>(new glass(1.5)));
    
    hitable* list[501]={};
    int count =0;
    hitable_list* world= scene(list, count);

    glm::vec3 lookfrom(8,2,3);
    glm::vec3 lookat(0,0,0);
    float distance_to_focal = glm::length(lookat-lookfrom);
    float aperture = 0.1f;

    camera cam(lookfrom,lookat,glm::vec3(0,1,0),60.0f, nx/float(ny), distance_to_focal,aperture);

    for (int j=ny-1 ; j >= 0; j--){
        for (int i=0; i< nx ; i++){
     
            glm::vec3 col(0.0,0.0,0.0);
            for (int s=0;s<ns;s++){
                float u = float(i+drand48())/float(nx);
                float v = float(j+drand48())/float(ny);
                ray r = cam.get_ray(u,v);
                col+=color(r,world,0);
            }
            col/=float(ns);
            col= glm::vec3(sqrt(col.x),sqrt(col.y),sqrt(col.z));
      
            int ir = static_cast<int>(255 * col[0]);
            int ig = static_cast<int>(255 * col[1]);
            int ib = static_cast<int>(255 * col[2]);
            outFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    //free memory
    delete world;
    for(int i=0;i<count;i++){
        delete list[i];
    }

    outFile.close();
}