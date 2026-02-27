@echo off
REM Build script for Windows using MinGW

REM Create build directory
if not exist ..\build mkdir ..\build

echo Building the engine...

REM Include directories
set GLAD_INC=-I"..\vendor\glad\include"
set STB_INC=-I"..\vendor\stb_image\include"
set GLM_INC=-I"..\vendor\glm"
set GLFW_INC=-I"..\vendor\glfw/include"

REM Copy shaders
xcopy /E /I /Y src\shaders ..\build\shaders

REM Compile each source file
g++ -c ..\vendor\glad\src\glad.c %GLAD_INC% -o ..\build\glad.o

g++ -std=c++20 -DBUILDING_XETHIUMLIB -c src\engine.cpp -Iinclude -Iheaders %GLAD_INC% %GLFW_INC% %STB_INC% %GLM_INC% -Wall -o ..\build\engine.o
g++ -std=c++20 -c src\platform.cpp -Iheaders %GLFW_INC% -Wall -o ..\build\platform.o
g++ -std=c++20 -c src\renderer.cpp -Iheaders %GLFW_INC% %GLAD_INC% %STB_INC% %GLM_INC% -Wall -o ..\build\renderer.o
g++ -std=c++20 -c src\shader.cpp -Iheaders %GLAD_INC% %GLM_INC% -Wall -o ..\build\shader.o
g++ -std=c++20 -c ..\vendor\stb_image\stb_image.cpp %STB_INC% -o ..\build\stb_image.o
g++ -std=c++20 -c src\texture.cpp -Iheaders %GLAD_INC% %STB_INC% -o ..\build\texture.o
g++ -std=c++20 -c src\meshFactory.cpp -Iheaders %GLAD_INC% %STB_INC% -o ..\build\meshFactory.o
g++ -std=c++20 -c src\gameObject.cpp -Iheaders %GLAD_INC% %STB_INC% %GLM_INC% -o ..\build\gameObject.o

REM Link into a DLL
g++ -shared -o ..\build\engine.dll ^
    ..\build\engine.o ^
    ..\build\platform.o ^
    ..\build\renderer.o ^
    ..\build\glad.o ^
    ..\build\shader.o ^
    ..\build\stb_image.o ^
    ..\build\texture.o ^
    ..\build\meshFactory.o ^
    ..\build\gameObject.o ^
    -L..\vendor\glfw/lib\ ^
    -Wl,--out-implib,..\build\engine.lib ^
    -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 -lkernel32 -g

echo Engine build complete: ..\build\engine.dll
pause
