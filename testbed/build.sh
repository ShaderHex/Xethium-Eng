#!/bin/bash

BUILD_DIR="../build"
mkdir -p $BUILD_DIR

CXX=g++

SRC="src/main.cpp"
ENGINE_INC="-I../engine/include"
GLFW_INC="-I../vendor/glfw/include"
GLAD_INC="-I../vendor/glad/include"
GLAD_SRC="../vendor/glad/src/glad.c"
INCLUDES="$ENGINE_INC $GLFW_INC $GLAD_INC"
ENGINE_LIB="-L$BUILD_DIR -lengine"
LIBS="-lglfw -ldl -lGL -lpthread"
CXXFLAGS="-std=c++20 -Wall"
OUT="$BUILD_DIR/testbed"
RPATH="-Wl,-rpath,\$ORIGIN"

$CXX $CXXFLAGS $SRC $GLAD_SRC $INCLUDES $ENGINE_LIB $LIBS -o $OUT $RPATH
