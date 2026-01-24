#!/bin/bash

mkdir -p ../build

echo "Building the engine..."

GLAD_INC="-I../vendor/glad/include"

STB_INC="-I../vendor/stb_image/include/"

GLM_INC="-I../vendor/glm/"

cp -r src/shaders ../build/

gcc -c ../vendor/glad/src/glad.c \
    $GLAD_INC \
    -fPIC \
    -o ../build/glad.o

g++ -std=c++20 -c src/engine.cpp \
    -Iinclude \
    -Iheaders \
    $GLAD_INC \
    $STB_INC \
    $GLM_INC \
    -Wall \
    -fPIC \
    -o ../build/engine.o

g++ -std=c++20 -c src/platform.cpp \
    -Iheaders \
    -Wall -fPIC \
    -o ../build/platform.o

g++ -std=c++20 -c src/renderer.cpp \
    -Iheaders \
    $GLAD_INC \
    $STB_INC \
    -Wall -fPIC \
    -o ../build/renderer.o

g++ -std=c++20 -c src/shader.cpp \
    -Iheaders \
    $GLAD_INC \
    -Wall -fPIC \
    -o ../build/shader.o

g++ -std=c++20 -c ../vendor/stb_image/stb_image.cpp \
    $STB_INC \
    -fPIC \
    -o ../build/stb_image.o

g++ -std=c++20 -c src/texture.cpp \
    -Iheaders \
    $GLAD_INC \
    $STB_INC \
    -fPIC \
    -o ../build/texture.o

g++ -std=c++20 -c src/meshFactory.cpp \
    -Iheaders \
    $GLAD_INC \
    $STB_INC \
    -fPIC \
    -o ../build/meshFactory.o

g++ -shared \
    -o ../build/libengine.so \
    ../build/engine.o \
    ../build/platform.o \
    ../build/renderer.o \
    ../build/glad.o \
    ../build/shader.o \
    ../build/stb_image.o \
    ../build/texture.o \
    ../build/meshFactory.o \
    -lglfw -lGL


echo "Engine build complete: ../build/libengine.so"
