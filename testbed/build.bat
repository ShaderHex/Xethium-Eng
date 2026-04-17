@echo off

set BUILD_DIR=..\build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

set SRC=src\main.cpp

set GLAD_SRC=..\vendor\glad\src\glad.c

set IMGUI_SRC=^
..\vendor\imgui\imgui.cpp ^
..\vendor\imgui\imgui_draw.cpp ^
..\vendor\imgui\imgui_tables.cpp ^
..\vendor\imgui\imgui_widgets.cpp ^
..\vendor\imgui\backends\imgui_impl_glfw.cpp ^
..\vendor\imgui\backends\imgui_impl_opengl3.cpp


set ENGINE_INC=-I"..\engine\include"
set ENGINE_HEADERS=-I"..\engine\headers"
set GLFW_INC=-I"..\vendor\glfw\include"
set GLAD_INC=-I"..\vendor\glad\include"
set GLM_INC=-I"..\vendor\glm"
set IMGUI_INC=-I"..\vendor\imgui"
set IMGUI_BACKEND_INC=-I"..\vendor\imgui\backends"

set ENGINE_LIB=-L%BUILD_DIR% -lengine


set LIBS=-lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 -lkernel32

set CXXFLAGS=-std=c++20 -Wall -g

set OUT=%BUILD_DIR%\testbed.exe

g++ %CXXFLAGS% ^
%ENGINE_INC% %ENGINE_HEADERS% %GLFW_INC% %GLAD_INC% %GLM_INC% %IMGUI_INC% %IMGUI_BACKEND_INC% ^
%SRC% ^
%GLAD_SRC% ^
%IMGUI_SRC% ^
%ENGINE_LIB% ^
-L"..\vendor\glfw\lib" ^
%LIBS% ^
-o %OUT%

echo Build complete: %OUT%
