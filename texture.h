#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <glm/glm.hpp>


class texture{
    public:
        virtual ~texture() = default;
        virtual glm::vec3 value(float u, float v, const glm::vec3 &point) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const glm::vec3& albedo) : albedo(albedo) {}
        solid_color(float r, float g, float b) : solid_color(glm::vec3(r,g,b)){}
        glm::vec3 value(float u, float v, const glm::vec3& p) const override{
            return albedo;
        }

    private:
        glm::vec3 albedo;
};

class checker_texture :  public texture {
    public:
        checker_texture(float scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) :  inv_scale(1.0/scale),even(even), odd(odd){}
        checker_texture(float scale, glm::vec3 col1, glm::vec3 col2) : checker_texture(scale, std::make_shared<solid_color>(col1), std::make_shared<solid_color>(col2)){}
        glm::vec3 value(float u, float v, const glm::vec3 &point) const override {
            auto x_int = int(std::floor(inv_scale * point.x));
            auto y_int = int(std::floor(inv_scale * point.y));
            auto z_int = int(std::floor(inv_scale * point.z));

            bool isEven = (x_int+y_int+z_int)%2 == 0 ;
            return isEven ? even->value(u,v,point) : odd->value(u,v,point);
        }


    private:
        float inv_scale;
        std::shared_ptr<texture> odd;
        std::shared_ptr<texture> even;

};

#endif
