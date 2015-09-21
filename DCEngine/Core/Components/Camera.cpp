#include "Camera.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {

  Camera::Camera(Entity & owner) : Component(std::string("Camera"), owner) {
  }

	void Camera::TransformUpdate(glm::vec3 &PositionInput, GLfloat RotationDegree)
	{
		Position.x = PositionInput.x;
		Position.y = PositionInput.y;
		Position.z = PositionInput.z;

		Roll = RotationDegree;
	}

  void Camera::Initialize() {
    auto gameObjOwner = (GameObject*)Owner();
	if (Transform_ != gameObjOwner->getComponent<Transform>())
	{
	  trace << "Camera: Intialize failed. No Transform component";
	}
	Yaw = -90.0f;
	Pitch = 0.0f;
	Roll = 0.0f;

	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	Right = glm::vec3(1.0f, 0.0f, 0.0f);
  }

  void Camera::Update() 
  {
	Roll = Transform_->Rotation.z;
	UpdateCameraVectors();
  }

  void Camera::Serialize(Json::Value & root) {
  }

  void Camera::Deserialize(Json::Value & root) {
  }

  glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Transform_->Translation, Transform_->Translation + Front, Up);
  }

  void Camera::UpdateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
	glm::vec3 up;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

	up.x = cos(glm::radians(Roll));
	up.y = sin(glm::radians(Roll));
	up.z = sin(glm::radians(Roll));
    // Also, recalculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, Up));
    Up = glm::normalize(up);
  }

  glm::mat4 Camera::GetProjectionMatrix() {
    if (Projection = Orthographic) {
      //First parameter: left coordinate second: right coordinate third: bottom coordinate fourth: top coordinate fifth: near plane sixthL far plane
      return glm::ortho(Size * 40 / 2, Transform_->Translation.x + Size * 40 / 2, Size * 40 / 2, Size * 40 / 2, NearPlane, FarPlane);
    }
    else if (Projection = Perspective) {
      return glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    }
    else

    {
      trace << "Camera: Wrong settings in Camera Projection settings. It should be one of the following: Orthographic or Perspective\n";
    }
  }

}
