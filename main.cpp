#include <iostream>
#include <fstream>

#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"

static const std::string FILENAME = "render.ppm";

static const int SAMPLES_PER_PIXEL = 10;

static const int WIDTH = 800;
static const int HEIGHT = 400;
static const Vec3 bg_color(0.5f, 0.7f, 1.f);

/**
 * Normalizes value in given min/max range
 * https://stats.stackexchange.com/questions/70801/how-to-normalize-data-to-0-1-range
 */
float normalize(float in,float min, float max){
    return (in-min)/(max-min);
}
// http://mathworld.wolfram.com/SpherePointPicking.html
Vec3 random_point_on_unit_sphere(){
    auto phi = static_cast<float>(drand48() * 2 * M_PI);
    auto u = static_cast<float>(drand48() * 2 - 1);
    float t = sqrtf(1-u*u);
    return {t*cosf(phi),t*sinf(phi),u};
}

/*
 * right hand coordinate system (+z is out of monitor): camera is at (0,0,0) and looks to a screen centered at (0,0,-1)
 * Screen has edges at (w/h ratio) top left:(-2,1,-1), top right: (2,1,-1) and symmetric down.
 */
Vec3 color(const ray& r, hitable *world){
    //sphere
    hit_record rec;
    if(world->hit(r,0.0001f,MAXFLOAT,rec)) {
        Vec3 target = rec.p + rec.normal + random_point_on_unit_sphere();
        return 0.5 * color(ray(rec.p,target-rec.p),world);
        //return 0.5 * Vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
    }

    //background

    float t = normalize(r.getDirection().y,-1,1);
    //std::cout << r.getDirection().y << std::endl;
    return (1-t)*Vec3(1.f,1.f,1.f)+(t)*bg_color; //lerp of default color
}

void render(camera cam, std::ofstream &file, hitable *world){

    srand(static_cast<unsigned int>(time(nullptr)));

    int buffer_index = 0;

    for (int j = 0; j < HEIGHT; ++j) {

        for (int i = 0; i < WIDTH; ++i) {

            Vec3 col(0,0,0);
            for (int k = 0; k < SAMPLES_PER_PIXEL; ++k) {
                //ToDo: compensate for FOV distortion!!
                float u = (float) (i+drand48()) / (float) WIDTH;
                float v = (float) (j+drand48()) / (float) HEIGHT;
                ray r = cam.get_ray(u,v);
                col += color(r,world);
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

    hitable *list[2];
    list[0] = new sphere(Vec3(0,0,-1),0.5f);
    list[1] = new sphere(Vec3(0,-100.5f,-1),100);
    hitable *world = new hitable_list(list,2);

    camera cam;

    render(cam,file,world);


    free(list[0]);
    free(list[1]);
    free(world);
    file.close();
    return 0;
}