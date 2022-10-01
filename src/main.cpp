#include "utils/rtweekend.h"

#include "utils/color.h"
#include "utils/hittable_list.h"
#include "primitives/sphere.h"
#include "primitives/camera.h"
#include "utils/material.h"
#include "utils/scenes.h"

// time
#include <chrono>
#include <sys/time.h>
#include <ctime>

#include <iostream>
#include <vector>

// time
using std::cout;
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

// Returns a color for a given ray r
color ray_color(const ray &r, const hittable &world, int depth)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

// Scenes

int main()
{
    // set random seed
    srand(69);

    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 512;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 16;
    const int max_depth = 2;

    // World
    auto world = Scenes::floor_sphere_scene();

    srand(time(NULL));

    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 12.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 19, aspect_ratio, aperture, dist_to_focus);

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
        eta = static_cast<float>((end_time - start_time)) *
              static_cast<float>(j) /
              static_cast<float>(image_height - j) /
              1000.0;

        std::cerr << "\rETA: " << eta << " sec "
                  << " | " << (image_height - j) << "/" << image_height << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                // Screen UV coordinates
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);

                // Add the color of every sample to current pixels color
                pixel_color += ray_color(r, world, max_depth);
            }
            // this function averages the pixel_color based on the number of samples per pixel
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    std::cerr << "\nDone. in " << (end_time - start_time) / 1000 << " seconds\n";
}