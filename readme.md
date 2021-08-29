# Controls

WASD for horizontal movement
SPACE and LSHIFT for vertical movement

1 increases render distance
2 decreases render distance

# Building Source

## Using WSL
1. run lib.sh (requires cmake, python, and mingw to be installed)
2. use make
> - clean, deletes all build files
> - build, compiles project in debug mode
> - release, cleans and compiles project in release mode
> - run, runs the output executable

## Using other
1. create a folder named "include" in the root directory
2. inside the include folder create four folders: "GL", "GLFW", "KHR", and "stb"
3. download gl3w
	- run gl3w_gen.py
	- compile the generated files as a shared library copying the dll into "windows" in the root directory and the header files from "GL" into "include/GL" and header files from "KHR" into "include/KHR"
4. download glfw or build glfw from source, copying the dll into "windows" in the root directory and "glfw3.h" and "glfw3native.h" into "include/GLFW"
5. download stb_image, stb_image_resize, stb_truetype, and stb_perlin from https://github.com/nothings/stb and place them inside of "include/stb"


