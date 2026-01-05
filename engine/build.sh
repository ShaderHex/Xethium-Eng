#!/bin/bash

mkdir -p ../build

echo "Building the engine..."

GLAD_INC="-I../vendor/glad/include"

gcc -c ../vendor/glad/src/glad.c \
    $GLAD_INC \
    -fPIC \
    -o ../build/glad.o

g++ -std=c++20 -c src/engine.cpp \
    -Iinclude \
    -Iheaders \
    $GLAD_INC \
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
    -Wall -fPIC \
    -o ../build/renderer.o

g++ -shared \
    -o ../build/libengine.so \
    ../build/engine.o \
    ../build/platform.o \
    ../build/renderer.o \
    ../build/glad.o \
    -lglfw -lGL


echo "Engine build complete: ../build/libengine.so"
