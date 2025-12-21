#!/bin/bash

mkdir -p ../build

echo "Building the engine..."

g++ -std=c++20 -Iinclude -Wall -fPIC -c src/application.cpp -o ../build/application.o

g++ -shared -o ../build/libengine.so ../build/*.o

echo "Engine build complete: ../build/libengine.so"
