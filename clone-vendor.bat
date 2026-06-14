@echo off
echo [XENGINE] Cloning dependencies...

if not exist "vendor" mkdir vendor

:: Assimp
if exist "vendor\assimp" (
    echo [XENGINE] assimp already exists, skipping...
) else (
    echo [XENGINE] Cloning assimp v5.3.1...
    git clone --depth 1 --branch v5.3.1 https://github.com/assimp/assimp.git vendor\assimp
    if errorlevel 1 (
        echo [XENGINE] ERROR: Failed to clone assimp
        exit /b 1
    )
    echo [XENGINE] assimp cloned successfully
)

echo [XENGINE] Done! All dependencies are ready.
pause