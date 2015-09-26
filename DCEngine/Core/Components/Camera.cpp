#include "Camera.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {

	Camera::Camera(Entity & owner) : Component(std::string("Camera"), owner) {
	}



	void Camera::Initialize() {
		auto gameObjOwner = (GameObject*)Owner();
		Transform_ = gameObjOwner->getComponent<Transform>();
		if (Transform_ != gameObjOwner->getComponent<Transform>())
		{
			trace << "Camera: Intialize failed. No Transform component";
			return;
		}
		Yaw = -90.0f;
		Pitch = 0.0f;
		Roll = 90.0f;

		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		Right = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	void Camera::Update()
	{
		Roll = this->Roll + Transform_->Rotation.z;
		trace << FieldOfView << "\n";
		if (FieldOfView >= 90.0f)
		{
			FieldOfView = 89;
		}
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
		if (Projection == ProjectionMode::Orthographic) {

			//First parameter: left coordinate second: right coordinate third: bottom coordinate fourth: top coordinate fifth: near plane sixthL far plane
			//trace << Transform_->Translation.x << Transform_->Translation.y << "\n";

			if (Size >= 99)
			{
				Size = 1;
			}
			return glm::ortho(Transform_->Translation.x - (WindowsWidth / WindowsHeight) * (100 - Size),
				Transform_->Translation.x + (WindowsWidth / WindowsHeight) * (100 - Size),
				Transform_->Translation.y - (WindowsHeight / WindowsWidth) * (100 - Size),
				Transform_->Translation.y + (WindowsHeight / WindowsWidth) * (100 - Size),
				NearPlane,
				FarPlane);
		}
		else if (Projection == ProjectionMode::Perspective) {
			return glm::perspective(FieldOfView, 1.0f, NearPlane, FarPlane);
		}
		else
		{
			trace << "Camera: Wrong settings in Camera Projection settings. It should be one of the following: Orthographic or Perspective\n";

		}
	}
	void Camera::OnLogicUpdate(Events::LogicUpdate* event)
	{
		Update();
	}

}


// Comment Format
/**************************************************************************/
/*!
\brief Calculates the current projection matrix and returns it.
\return The projection matrix (Orthographic or Perspective)
\note
*/
/**************************************************************************/
