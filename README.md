# **GHD Path Tracer**


<!-- ABOUT THE PROJECT -->
## About The Project
A simple Path Tracer built for educational purposes by following the ["Ray Tracing in one weekend"](https://raytracing.github.io/books/RayTracingInOneWeekend.html) Book By Peter Shirley. 

(Spoiler : Takes more then "a" weekend!)

[![Final Rendered Image][product-screenshot]](https://example.com)

The Rendered Image was a combination (Mean) of 6 instances of the program running on different Threads (doesn't support internal multi-threading yet)

Total Render Time  : ```30 minutes @ 3.4 GHz```

Settings:
```c++
// Image
const auto aspect_ratio = 3.0 / 2.0;
const int image_width = 1024;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 50;
const int max_depth = 8;
```

### Built With

* [C++](https://www.cplusplus.com/https://www.cplusplus.com/)
* [Blender](https://www.blender.org/)
* [ImageMagick](https://imagemagick.org/index.php)

<!-- GETTING STARTED -->
# Getting Started

Follow the instructions given below to compile and run the program.

### Notes
* This project was developed and tested on ```Ubuntu 20``` using The included ```g++``` compiler.
* To use the image convertion tools you need to install [imagemagick](https://imagemagick.org/index.php)


## Preferred Method :
If You use **VS Code** and **Code Runner Extension**, the ```settings.json``` file contains the appropriate ```"code-runner.executorMap"``` command.
So just press ```Ctrl+Alt+N``` to automatically compile the code, save the ppm file and open it using ```"eog"``` image viewer.

## Manual Method :

### Compile
Compile ```main.cpp``` using the ```g++``` compiler.
```
g++ src/main.cpp -o exec/temp_output
```

### Run
After compiling the program, run the output binary and save the result to a ppm file using the command below :
```
./exec/temp_output > renders/{file_name}.ppm
```
Or if you want to save the result to a random file, use the following command (Linux only):
```
rnd=$(hexdump -n 16 -e '4/4 "%08X" 1 "\n"' /dev/urandom) && ./exec/temp_output > renders/$rnd.ppm
```

Then open the rendered ```.ppm``` file using your preferred Image Viewer.

## Tools
There are several tools available in this project.
They include tools for batch converting ```.ppm``` files to ```.jpg``` or ```.png```
and a python script to export XYZ and Scale attributes from a selected objects in blender.

all of these can be accessed in the ```tools``` folder.
<!-- ROADMAP -->
## TODO

- [ ] Multi-Threading
- [ ] I/O
- [ ] Frame-Buffer
- [ ] BVH
- [ ] Code cleanup

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!-- CONTACT -->
## Contact

Your Name - [@twitter_handle](https://instagram.com/gholamreza_dar) - gholamrezadar@gmail.com

Project Link: [https://github.com/Gholamrezadar/GHD-Path-Tracer](https://github.com/Gholamrezadar/GHD-Path-Tracer)

[license-url]: https://github.com/Gholamrezadar/GHD-Path-Tracer/blob/master/LICENSE.txt
[product-screenshot]: Final.jpg
