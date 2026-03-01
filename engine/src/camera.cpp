#include "camera/camera.h"

namespace Camera {

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect) {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

}