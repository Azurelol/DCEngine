#include "Camera.h"
#include "EngineReference.h"

namespace DCEngine {

  glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
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

  glm::mat4 Camera::GetProjectionMatrix() {
    if (Projection = Orthographic) {
      //First parameter: left coordinate second: right coordinate third: bottom coordinate fourth: top coordinate fifth: near plane sixthL far plane
      return glm::ortho(Position.x - CameraWidth / 2, Position.x + CameraWidth / 2, Position.y - CameraHeight / 2, Position.x + CameraHeight / 2, NearPlane, FarPlane);
    }
    else if (Projection = Perspective) {
      return glm::perspective(45.0f, CameraWidth / CameraHeight, 0.1f, 100.0f);
    }
    else

    {
      trace << "Camera: Wrong settings in Camera Projection settings. It should be one of the following: Orthographic or Perspective\n";
    }
  }

}