#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "math_helper.h"
#include "material.h"

static const std::string FILENAME = "render.ppm";

static const int SAMPLES_PER_PIXEL = 100;
static const int MAX_DEPTH = 200;

static const int WIDTH = 800;
static const int HEIGHT = 400;
static const Vec3 bg_color(0.5f, 0.7f, 1.f);
static const float SHADOW_BIAS = 0.0001f;



/*
 * right hand coordinate system (+z is out of monitor): camera is at (0,0,0) and looks to a screen centered at (0,0,-1)
 * Screen has edges at (w/h ratio) top left:(-2,1,-1), top right: (2,1,-1) and symmetric down.
 */
Vec3 color(const ray& r, const std::shared_ptr<hitable_list> &world, int depth){
    //sphere
    hit_record rec;
    if(world->hit(r,SHADOW_BIAS,MAXFLOAT,rec)) {
        ray scattered;
        Vec3 attenuation;
        //rec.mat_ptr->
        if(depth < MAX_DEPTH && rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
            return attenuation*color(scattered,world,depth+1);
        }else{
            return Vec3(0,0,0);
        }

        //Vec3 target = rec.p + rec.normal + random_point_on_unit_sphere();
        //return 0.5 * color(ray(rec.p,target-rec.p),world);
    }

    //background

    float t = normalize(r.getDirection().y,-1,1);
    //std::cout << r.getDirection().y << std::endl;
    return (1-t)*Vec3(1.f,1.f,1.f)+(t)*bg_color; //lerp of default color
}

void render(camera cam, std::ofstream &file, const std::shared_ptr<hitable_list> &world){

    srand(static_cast<unsigned int>(time(nullptr)));

    int buffer_index = 0;

    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            Vec3 col(0,0,0);
            for (int k = 0; k < SAMPLES_PER_PIXEL; ++k) {
                float u = (float) (i+drand48()) / (float) WIDTH;
                float v = (float) (j+drand48()) / (float) HEIGHT;
                ray r = cam.get_ray(u,v);
                col += color(r,world,0);
            }

            col /= SAMPLES_PER_PIXEL;

            col = sqrtvec(col); //Gamma correction

            //col is in range 0-1 but we want RGB in range 0-255
            int ir = uint8_t(255 * col.x);
            int ig = uint8_t(255 * col.y);
            int ib = uint8_t(255 * col.z);


            buffer_index += 3;
            file << ir << " " << ig << " " << ib << "\n";
        }
        std::cout << "\r[Info]:\tProgress: " << (((float)j)/((float)HEIGHT))*100 << "%";
    }
    std::cout << std::endl;//flush and add newline
}


int main() {
    std::ofstream file(FILENAME);

    //PPM Header: P3 is the magic number for ppm in ASCII format, followed by width and height, followed by 255, the max
    file << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    auto mat_lamb1 = std::make_shared<lambertian>(lambertian(Vec3(0.1f,0.3f,0.3f)));
    auto mat_lamb2 = std::make_shared<lambertian>(lambertian(Vec3(0.9f,0.3f,0.3f)));
    auto mat_metal1 = std::make_shared<metal>(metal(Vec3(0.8f,0.8f,0.8f),2));
    auto mat_metal2 = std::make_shared<metal>(metal(Vec3(0.1f,0.3f,0.5f),10));

    std::vector<std::shared_ptr<hitable>> elements;
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(0,0,-1),0.5f,mat_lamb1)));
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(-1,0,-1),0.5f,mat_metal2)));
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(1,0,-1),0.5f,mat_metal1)));
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(0,-100.5f,-1),100,mat_lamb2)));
    std::shared_ptr<hitable_list> world = std::make_shared<hitable_list>(elements);


    camera cam;

    std::cout << "[INFO]:\tInitialization successful, start rendering" << std::endl;

    render(cam,file,world);


    file.close();
    return 0;
}