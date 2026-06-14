@echo off
echo [XETHIUM] Cloning dependencies...

if not exist "vendor" mkdir vendor

if exist "vendor\assimp" (
    echo [XETHIUM] assimp already exists, skipping...
) else (
    echo [XETHIUM] Cloning assimp v5.3.1...
    git clone --depth 1 --branch v5.3.1 https://github.com/assimp/assimp.git vendor\assimp
    if errorlevel 1 ( echo [XETHIUM] ERROR: Failed to clone assimp & exit /b 1 )
    echo [XETHIUM] assimp cloned successfully
)

if exist "vendor\glfw" (
    echo [XETHIUM] glfw already exists, skipping...
) else (
    echo [XETHIUM] Cloning glfw 3.4...
    git clone --depth 1 --branch 3.4 https://github.com/glfw/glfw.git vendor\glfw
    if errorlevel 1 ( echo [XETHIUM] ERROR: Failed to clone glfw & exit /b 1 )
    echo [XETHIUM] glfw cloned successfully
)

if exist "vendor\glm" (
    echo [XETHIUM] glm already exists, skipping...
) else (
    echo [XETHIUM] Cloning glm 1.0.1...
    git clone --depth 1 --branch 1.0.1 https://github.com/g-truc/glm.git vendor\glm
    if errorlevel 1 ( echo [XETHIUM] ERROR: Failed to clone glm & exit /b 1 )
    echo [XETHIUM] glm cloned successfully
)

if exist "vendor\imgui" (
    echo [XETHIUM] imgui already exists, skipping...
) else (
    echo [XETHIUM] Cloning imgui v1.91.6...
    git clone --depth 1 --branch v1.91.6 https://github.com/ocornut/imgui.git vendor\imgui
    if errorlevel 1 ( echo [XETHIUM] ERROR: Failed to clone imgui & exit /b 1 )
    echo [XETHIUM] imgui cloned successfully
)

if exist "vendor\stb" (
    echo [XETHIUM] stb already exists, skipping...
) else (
    echo [XETHIUM] Cloning stb (pinned commit)...
    git clone https://github.com/nothings/stb.git vendor\stb
    if errorlevel 1 ( echo [XETHIUM] ERROR: Failed to clone stb & exit /b 1 )
    cd vendor\stb
    git checkout 5736b15f7ea0ffb08dd38af21067c314d6a3aae9
    cd ..\..
    echo [XETHIUM] stb cloned successfully
)

echo.
echo [XETHIUM] All dependencies ready in vendor/
pause