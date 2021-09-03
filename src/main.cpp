#include "utils/rtweekend.h"

#include "utils/color.h"
#include "utils/hittable_list.h"
#include "primitives/sphere.h"

#include <iostream>

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
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // very large sphere faking a plane
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    // horizontal vector. W vector
    auto horizontal = vec3(viewport_width, 0, 0);
    // horizontal vector. W vector
    auto vertical = vec3(0, viewport_height, 0);
    // Starting vector. used later to calculate every other vector on the viewport
    // Like this : "lower_left_corner + u*horizontal + v*vertical"
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);


    // Render

    // Write PPM Header
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // Loop over pixels
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            // Viewports UV coordinate
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            // A ray going through the current pixel
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r, world);

            // Write to output
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}