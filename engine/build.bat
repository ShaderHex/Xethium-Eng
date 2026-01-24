@echo off

mkdir ..\build

echo Building the engine...

clang++ -std=c++20 -Iinclude -Wall -c src\application.cpp -o ..\build\application.obj

clang++ -shared -o ..\build\engine.dll ..\build\application.obj

echo Engine build complete: ..\build\engine.dll
