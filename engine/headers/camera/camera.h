#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Camera {

class Camera {
public:
    glm::vec3 position = {0.0f, 0.0f, 5.0f};
    glm::vec3 target = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    float fov = 70.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float aspect);

}; 

}