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

//time
using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

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

int main()
{

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 25;

    // World
    // W1) a plane and a sphere on top
    
    hittable_list world;
    auto material_ground = make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_ball = make_shared<lambertian>(color(0.8, 0.15, 0.05));
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5, material_ball));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));

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

    // Camera
    camera cam;

    // Render

    // Write PPM Header
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // time before rendering
    auto start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    auto end_time = start_time;
    auto eta = 0.0;
    // Loop over pixels
    for (int j = image_height - 1; j >= 0; --j)
    {
        // std::cerr << "\rScanlines Remaining: " << j << ' ' <<std::endl;
        end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        // time_remaining = time_elapsed *
        //                  row_remaining /
        //                  rows_elapsed
        eta = ((end_time-start_time)/1000.0) *
              j /
              (image_height-j);
              

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