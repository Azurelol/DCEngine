#include "Camera.h"

namespace DCEngine {

  glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position +Front, Up);
  }

  void Camera::UpdateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also, recalculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }

}