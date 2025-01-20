#include "raylib.h"
#include "Camera.h"
#include <iostream>

void CameraWrapper::Init() {
    camera = {0};
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
     std::cout << "Position: (" 
          << camera.position.x << ", " 
          << camera.position.y << ", " 
          << camera.position.z << ")" 
          << std::endl;    
    std::cout<<"Camera init done!\n";
}
