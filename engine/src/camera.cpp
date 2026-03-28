#include "camera/camera.h"

namespace Camera {

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect) {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::UpdateVectors() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(dir);
}

}