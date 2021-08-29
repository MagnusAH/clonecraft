#!/bin/bash

mkdir include windows

#gl3w

mkdir include/KHR include/GL
git clone https://github.com/skaslev/gl3w.git
cd gl3w && python gl3w_gen.py && cd ..

cp -r gl3w/include .
x86_64-w64-mingw32-gcc gl3w/src/gl3w.c -Iinclude/ -o windows/gl3w.dll -shared -fpic

rm -rf gl3w

#glfw

mkdir include/GLFW

git clone https://github.com/glfw/glfw.git

cp glfw/include/GLFW/*.h include/GLFW/

mkdir glfw/build/

cmake -DBUILD_SHARED_LIBS=ON -S glfw -B glfw/build/ -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake
make -j -C glfw/build/

cp glfw/build/src/glfw3.dll windows/
cp glfw/build/src/libglfw3dll.a windows/

rm -rf glfw

#stb_image

mkdir include/stb

git clone https://github.com/nothings/stb.git

cp stb/stb_image.h include/stb
cp stb/stb_image_resize.h include/stb
cp stb/stb_truetype.h include/stb
cp stb/stb_perlin.h include/stb

rm -rf stb