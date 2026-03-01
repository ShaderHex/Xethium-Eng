REM Windows build for testbed using MinGW

REM Build directory
set BUILD_DIR=..\build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

REM Source and includes
set SRC=src\main.cpp
set ENGINE_INC=-I"..\engine\include"
set GLFW_INC=-I"..\vendor\glfw\include"
set GLAD_INC=-I"..\vendor\glad\include"
set GLAD_SRC=..\vendor\glad\src\glad.c
set GLM_INC=-I"..\vendor\glm"

REM Engine library
set ENGINE_LIB=-L%BUILD_DIR% -lengine
set ENGINE_HEADERS=-I"..\engine\headers"

REM Other libraries
set LIBS=-lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 -lkernel32

REM Compiler flags
set CXXFLAGS=-std=c++20 -Wall -g

REM Output
set OUT=%BUILD_DIR%\testbed.exe

REM Compile and link
g++ %CXXFLAGS% %SRC% %GLAD_SRC% %ENGINE_INC% %ENGINE_HEADERS% %GLFW_INC% %GLAD_INC% %GLM_INC% ..\build\engine.lib -L..\vendor\glfw/lib\ %LIBS% -o %OUT%
echo Build complete: %OUT%
