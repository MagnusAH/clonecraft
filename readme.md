Due to matrix math implementations the project currently only runs on x86_64 and requires fma support (any cpu from the past 5 years should have this)

# Controls

WASD for horizontal movement
SPACE and LSHIFT for vertical movement

1 increases render distance
2 decreases render distance

# Building Source

## Using WSL
1. run lib.sh (requires cmake, python, and mingw to be installed in the linux distro)
2. use make (static linking can be disabled if libstdc++-6.dll and libgcc_s_seh-1.dll are in path or the same directory as the executable)
> - clean, deletes all build files
> - build, compiles project in debug mode
> - release, cleans and compiles project in release mode
> - run, runs the output executable

## Using other
1. create a folder named "include" in the root directory
2. inside the include folder create four folders: "GL", "GLFW", "KHR", and "stb"
3. download gl3w
	1. run gl3w_gen.py
	2. compile the generated files as a shared library copying the dll into "windows" in the root directory, the header files from "GL" into "include/GL", and the header files from "KHR" into "include/KHR"
4. download glfw or build glfw from source, copying the dll into "windows" in the root directory and "glfw3.h" and "glfw3native.h" into "include/GLFW"
5. download stb_image, stb_image_resize, stb_truetype, and stb_perlin from https://github.com/nothings/stb and place them inside of "include/stb"
6. libraries
	- library directory is "windows"
	- libraries are "glfw" and "gl3w"
7. includes
	- include directory is "include"
8. other
	- sse4.2, avx2, and fma must be supported by the compiler

# Useful Things

## ChunkMap

> 3D hashmap, no hash function
> - bucket list is a "3D" array (1D but mapped to 3D, would probably be best to be truely 3D if the hashmap were to be resized) 
> - coordinates in 3D space are modulated by the hashmap size in the corresponding axis to find the coordinates in the hashmap
> - could be extended to float coordinates by treating the bits as an int

## Matrix4f

> 4x4 matrix class
> - inverse function beats out glm in performance
> - transpose function is probably one of the fastest ways to transpose a 4x4 matrix on x86_64
> - multiplication function is garbage and I need to rewrite it, \_mm_set() is a function you want to avoid using as much as possible
