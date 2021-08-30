#include "color.h"
#include "vec3.h"

#include <iostream>

int main()
{

    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render

    // PPM Header
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // Pixels
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            // pixel i,j 's color
            color pixel_color(
                double(i) / (image_width - 1),
                double(j) / (image_height - 1),
                0.25);

            // Write to output
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}