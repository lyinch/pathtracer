#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <chrono>
#include <cfloat>

#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"
#include "math_helper.h"
#include "material.h"
#include "bvh_node.h"

static const std::string FILENAME = "render.ppm";

static const int SAMPLES_PER_PIXEL = 100;
static const int MAX_DEPTH = 10;

static const int WIDTH = 800;
static const int HEIGHT = 400;
static const Vec3 bg_color(0.5f, 0.7f, 1.f);
static const float SHADOW_BIAS = 0.0001f; //offset after a hit to prevent self intersection



/*
 * The main part of the rendering algorithms. Takes a ray, calculates the intersection of the ray with the scene
 * and recursively scatters the ray from the objects until a maximal depth. We have not explicit light sampling, and
 * therefore have very slow convergence. If we don't hit anything, we return the background color.
 */
Vec3 color(const ray& r, const std::shared_ptr<hitable_list> &world, int depth){
    hit_record rec;

    if(world->hit(r,SHADOW_BIAS,FLT_MAX,rec)) {
        ray scattered;
        Vec3 attenuation;
        Vec3 emitted = rec.mat_ptr->emitted(rec.p);
        if(depth < MAX_DEPTH && rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
            return emitted + attenuation*color(scattered,world,depth+1);
        }else{
            return emitted;
        }
    }

    //background

    float t = normalize(r.getDirection().y,-2,2);
    if(t > 1){
       t = 1;
    }else if(t < 0){
        t = 0;
    }
    return Vec3(0,0,0); //black background because otherwise objects that do not emit (or hit an emitter) have a color
    //return (1-t)*Vec3(1.f,1.f,1.f)+(t)*bg_color; //lerp of background color
}


void render(camera cam, std::ofstream &file, const std::shared_ptr<hitable_list> &world){

    srand(static_cast<unsigned int>(time(nullptr)));

    int buffer_index = 0;

    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            Vec3 col(0,0,0);
            // We use distributed Raytracing. Send multiple samples per pixel for anti aliasing, variance and noise
            // reduction.
            for (int k = 0; k < SAMPLES_PER_PIXEL; ++k) {
                float u = (float) (i+drand48()) / (float) WIDTH;
                float v = (float) (j+drand48()) / (float) HEIGHT;
                ray r = cam.get_ray(u,v);
                col += color(r,world,0);
            }

            col /= SAMPLES_PER_PIXEL;

            col = sqrtvec(col); //Gamma correction

            //col is in range 0-1 but we want RGB in range 0-255
            int ir = uint16_t(255 * col.x);
            int ig = uint16_t(255 * col.y);
            int ib = uint16_t(255 * col.z);
            if(ir > 255)
                ir = 255;
            if(ig > 255)
                ig = 255;
            if(ib > 255)
                ib = 255;

            if(ir > 255|| ib > 255|| ig > 255){
                std::cout << "WTF" << std::endl;
            }
            buffer_index += 3;
            file << ir << " " << ig << " " << ib << "\n";
        }
        std::cout << "\r[INFO]:\tProgress: " << (((float)j)/((float)HEIGHT))*100 << "%";
        fflush(stdout);
    }
    std::cout << std::endl;//flush and add newline
}


int main() {
    std::ofstream file(FILENAME);

    //PPM Header: P3 is the magic number for ppm in ASCII format, followed by width and height, followed by 255, the max
    file << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    auto mat_lamb1 = std::make_shared<lambertian>(lambertian(Vec3(0.1f,0.3f,0.3f)));
    auto mat_lamb2 = std::make_shared<lambertian>(lambertian(Vec3(0.9f,0.3f,0.3f)));
    auto mat_metal1 = std::make_shared<metal>(metal(Vec3(0.8f,0.8f,0.8f),1));
    auto mat_metal2 = std::make_shared<metal>(metal(Vec3(0.1f,0.3f,0.1f),0.1));
    auto mat_diele1 = std::make_shared<dielectric>(dielectric(1.5f));

    auto mat_emitter = std::make_shared<diffuse_light>(diffuse_light(Vec3(4,4,4)));

    std::vector<std::shared_ptr<hitable>> elements;
    //elements.push_back(std::make_shared<moving_sphere>(moving_sphere(Vec3(-1,0,-1),Vec3(-0.7f,0,-1),0,1,0.5f,mat_metal2))); //left sphere
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(-1,0,-1),0.5f,mat_metal2))); //left sphere
    //elements.push_back(std::make_shared<sphere>(sphere(Vec3(0,0,-0.2f),0.5f,mat_lamb1))); //center sphere
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(0,0.1f,-1),0.5f,mat_emitter))); //center sphere
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(1,0,-1),0.5f,mat_diele1))); //right sphere
    elements.push_back(std::make_shared<sphere>(sphere(Vec3(0,-100.5f,-1),100,mat_lamb2)));

    bvh_node n = bvh_node(elements,0,4,0,0);
    //n.box._max = Vec3(10,10,10);
    std::vector<std::shared_ptr<hitable>> toRender;
    toRender.push_back(std::make_shared<bvh_node>(n));

    //std::shared_ptr<hitable_list> world = std::make_shared<hitable_list>(elements);
    std::shared_ptr<hitable_list> world = std::make_shared<hitable_list>(toRender);


    camera cam(Vec3(0,0,1),Vec3(0,0,-1),Vec3(0,1,0),90,float(WIDTH)/float(HEIGHT),0,1,0,1);
    //camera cam;

    std::cout << "[INFO]:\tInitialization successful, start rendering" << std::endl;

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    render(cam,file,world);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO]:\tRendering finished in " <<  std::chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count() << "s" << std::endl;

    file.close();
    return 0;
}