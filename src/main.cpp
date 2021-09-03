#include "utils/rtweekend.h"

#include "utils/color.h"
#include "utils/hittable_list.h"
#include "primitives/sphere.h"
#include "primitives/camera.h"

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
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main()
{

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 888;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // very large sphere faking a plane
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    // Render

    // Write PPM Header
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // time before rendering
    auto start_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    auto end_time = start_time;
    auto eta = 0;
    // Loop over pixels
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ';
        end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        // eta = ((end_time-start_time)/1000)*((image_height/(image_height -1 - j + 1))-1);
        eta = j*((end_time-start_time)/1000) / (image_height-j);
        std::cerr << "\rETA: " << eta << " sec " << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // Screen UV coordinates
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);

                // Add the color of every sample to current pixels color
                pixel_color += ray_color(r, world);
            }
            // this function averages the pixel_color based on the number of samples per pixel
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    end_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    std::cerr << "\nDone. in "<< (end_time - start_time)/1000<<" sec\n";
}