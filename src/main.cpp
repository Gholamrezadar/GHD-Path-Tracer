#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// Detects if a ray r, intersects with a sphere (center, radius)
bool hit_sphere(const point3& center, double radius, const ray& r){
    // Refer to 5.1 for the formula  
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(),r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto delta = b*b - 4*a*c;
    return (delta>0);
}

// Returns a color for a given ray r
color ray_color(const ray& r) {
    // Green if intersects with a sphere : c=(0,0,-1), r=0.5
    if (hit_sphere(point3(0,0,-1), 0.5, r))
        return color(0.1137, 0.8196, 0.6313);
    // Get unit vector of rays direction
    vec3 unit_direction = unit_vector(r.direction());
    // Remap ray.y from 0-1 to 0.5-1.0
    auto t = 0.5*(unit_direction.y() + 1.0);
    // Interpolate between white(bottom) and blue(top) based on y component of each ray
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main()
{

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

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
            color pixel_color = ray_color(r);

            // Write to output
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}