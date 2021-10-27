#include "utils/rtweekend.h"

#include "utils/color.h"
#include "utils/hittable_list.h"
#include "primitives/sphere.h"
#include "primitives/camera.h"
#include "utils/material.h"


//time
#include <chrono>
#include <sys/time.h>
#include <ctime>

#include <iostream>
#include <vector> 

//time
using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

std::vector < std::vector<double> > generate_spheres(double scale){
    // Z, Y, X, R
    std::vector < std::vector<double> > spheres{
        {-1,0,1,0.5},
        {0,0,0,0.5},
        {1,0,0,0.5},
    };

    return spheres;
}

// Returns a color for a given ray r
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

hittable_list GHD_scene(){
    hittable_list world;
    // Ground
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // list of x,y,z,R s
    std::vector < std::vector<double> > spheres = generate_spheres(1.0);

    // for each sphere on that list
    for (int i = 0; i < spheres.size(); i++)
    {
        auto choose_mat = random_double();
        point3 center(spheres[i][0],
                      spheres[i][1],
                      spheres[i][2]);

        // select a random material
        shared_ptr<material> sphere_material;
        if (choose_mat < 0.8) {
            // diffuse
            auto albedo = color::random() * color::random();
            sphere_material = make_shared<lambertian>(albedo);
        } else if (choose_mat < 0.95) {
            // metal
            auto albedo = color::random(0.5, 1);
            auto fuzz = random_double(0, 0.5);
            sphere_material = make_shared<metal>(albedo, fuzz);
        } else {
            // glass
            sphere_material = make_shared<dielectric>(1.5);
            
        }

        // create ith sphere and give it a random material 
        world.add(make_shared<sphere>(center, spheres[i][3], sphere_material));
        
    }
    return world;
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{

    // set random seed 
    srand (69);

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 5;

    // World
    // W1) a plane and a sphere on top
    // hittable_list world;
    // auto material_ground = make_shared<metal>(color(0.8, 0.8, 0.8));
    // auto material_ball = make_shared<lambertian>(color(0.8, 0.15, 0.05));
    // world.add(make_shared<sphere>(point3(0,0,-1), 0.5, material_ball));
    // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));

    // W2) three spheres - one lambertian center and two metals on each side
    // hittable_list world;
    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8));
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));
    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // W3) three spheres - one metal to the right and two dielectrics on its left
    // hittable_list world;
    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<dielectric>(1.5);
    // auto material_left   = make_shared<dielectric>(1.5);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // W3) three spheres - one metal to the right lambertian in the middle and a hollow glass on the left
    // hittable_list world;
    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<dielectric>(1.5);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // W4) FOV Test Scene
    // auto R = cos(pi/4);
    // hittable_list world;
    // auto material_left  = make_shared<lambertian>(color(0,0,1));
    // auto material_right = make_shared<lambertian>(color(1,0,0));
    // world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    // world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));

    //W5) Cover Scene - Random Spheres with random materials
    // auto world = random_scene();

    //W6) GHD Scene
    auto world = GHD_scene();

    srand(time(NULL));

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    // Write PPM Header
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // time before rendering
    auto start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    auto end_time = start_time;
    float eta = 0.0;
    // Loop over pixels
    for (int j = image_height - 1; j >= 0; --j)
    {
        // std::cerr << "\rScanlines Remaining: " << j << ' ' <<std::endl;
        end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        // time_remaining = time_elapsed *
        //                  row_remaining /
        //                  rows_elapsed
        eta = static_cast<float>((end_time-start_time)) *
              static_cast<float>(j) /
              static_cast<float>(image_height-j) /
              1000.0;
              

        std::cerr << "\rETA: " << eta << " sec " <<" | "<<(image_height-j)<<"/"<<image_height<< std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // Screen UV coordinates
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);

                // Add the color of every sample to current pixels color
                pixel_color += ray_color(r, world, max_depth);
            }
            // this function averages the pixel_color based on the number of samples per pixel
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    std::cerr << "\nDone. in "<< (end_time - start_time)/1000<<" sec\n";
}