#include <iostream>

int main()
{
    // Image
    const int image_width = 512;
    const int image_height = 512;

    // Render

    // ppm file header:
    // P3
    // image_width image_height
    // 255
    std::cout << "P3\n"
              << image_width << " " << image_height << "\n255\n";

    // ppm file data :
    // 255 255 255
    // 255 0   255
    // ...

    for (int j = 0; j < image_height; j++)
    {
        std::cerr << "\rScanlines remaining: " << image_height-1-j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_height - 1);
            double b = 0.25;

            int ir = static_cast<int>(255.0 * r);
            int ig = static_cast<int>(255.0 * g);
            int ib = static_cast<int>(255.0 * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    std::cerr << "\nDone.\n";

}