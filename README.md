# 🎥 Raytracer

A software Monte Carlo path tracing engine built in native C++ for rendering 3D scenes. This project was my first project in C++ and my introduction to several computer graphics concepts.

## 📝 Features

- Utilizes Monte Carlo integration, importance sampling, antialiasing, gamma correction, reflection, refraction, and scattering
- Features instancing, texture mapping, Perlin noise generation, emissive objects, defocus blur, motion blur, metal and dielectric materials, and more
- Implements a bounding volume hierarchy to accelerate ray-world intersections (4.6x speedup) and multithreading using a custom thread pool to further improve performance (16x speedup), optimizing performance by 98.6%
- Further profiling and optimizations in progress...

## 🔍 What I Learned

- Programming in C++, covering syntax, conventions, the standard library, and multithreaded programming
- Foundational concepts in computer graphics applied through raytracing
- The mathematical foundations behind ray tracing, including linear algebra, geometry, statistics, and optics
- Setting up build systems for C++ projects, including CMake and Visual Studio, and importing external libraries

## 🛠️ Build and Run

### Build with CMake

1. Make sure [CMake](https://cmake.org/) is installed on your local machine
2. Clone the repository to your local machine
3. Run the following instructions:

```shell
mkdir build
cd build
cmake ..
```

### Running the Program

A collection of sample scenes are provided in main.cpp showing off various features of the raytracer. Rendering parameters can be modified in the functions associated with each scene.

```shell
./Raytracer <scene>
# Example
./Raytracer 5
```

## 🖼️ Results

Note: Featured scenes rendered prior to Monte Carlo integration

### Sphere Scene

![Sphere Scene](./images/bouncing_spheres.png)

- 532 spheres with random positions, materials, and motions
- 1200x675 pixels, 500 sampled rays per pixel, 50 ray bounce limit
- Rendered in 282 seconds

### Cornell Box

![Cornell Box](./images/cornell_box.png)

- Bounce lighting from colored quads on transformed box instances
- 1200x1200 pixels, 500 sampled rays per pixel, 50 ray bounce limit
- Rendered in 1015 seconds

### All Features

![All Features](./images/all_features.png)
- Motion blur, thin mist, subsurface reflection, textures, instancing, and more
- 800x800 pixels, 10000 sampled rays per pixel, 40 bounce limit
- Rendered in 13 hours

## 📚 Reference

- [Ray Tracing in One Weekend](https://raytracing.github.io/)
