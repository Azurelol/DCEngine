#include "Camera.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
  
	Camera::Camera(Entity & owner) : Component(std::string("Camera"), owner) {
	}


	/**************************************************************************/
	/*!
	\brief Initializes the camera component and its settings.
	\note
	*/
	/**************************************************************************/
	void Camera::Initialize() {
		auto gameObjOwner = (GameObject*)Owner();
		TransformComponent = gameObjOwner->getComponent<Transform>();

		if (TransformComponent != NULL)
		{
			trace << "Camera::Initialize - Failed. No Transform component\n";
			return;
		}

		Connect(SpaceRef, Events::LogicUpdate, Camera::OnLogicUpdate);

		//Yaw = -90.0f;
		//Pitch = 0.0f;
		//Roll = 90.0f;

		//Front = glm::vec3(0.0f, 0.0f, -1.0f);
		//Up =    glm::vec3(0.0f, 1.0f, 0.0f);
		//Right = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	/**************************************************************************/
	/*!
	\brief The camera is updated on every LogicUpdate event.
	\param The update event.
	*/
	/**************************************************************************/
	void Camera::OnLogicUpdate(Events::LogicUpdate* event)
	{
		trace << "Camera::OnLogicUpdate::Updatelol";
		Update();
	}

	/**************************************************************************/
	/*!
	\brief Updates the camera. First...
	\note
	*/
	/**************************************************************************/
	void Camera::Update()
	{
		Roll = BaseRollVal + TransformComponent->Rotation.z;
		//trace << FieldOfView << "\n";
		if (FieldOfView >= 90.0f)
		{
			FieldOfView = 89;
		}
		UpdateCameraVectors();
	}



	/**************************************************************************/
	/*!
	\brief  Computes the camera view matrix based on its translation and
	Front and Up vectors.
	\return A 4x4 matrix that is used for the shader uniform "view".
	*/
	/**************************************************************************/
	glm::mat4 Camera::GetViewMatrix() {
		return glm::lookAt(TransformComponent->Translation, TransformComponent->Translation + Front, Up);
	}

	/**************************************************************************/
	/*!
	\brief  Computes the current camera vectors. If the camera projection mode
	is perspective, it calculates the front vector as well.
	*/
	/**************************************************************************/
	void Camera::UpdateCameraVectors() {
		// Calculate the new front vector
		glm::vec3 front;
		glm::vec3 up;

		if (Projection == ProjectionMode::Perspective) {
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
		}

		up.x = cos(glm::radians(Roll));
		up.y = sin(glm::radians(Roll));
		up.z = sin(glm::radians(Roll));
		// Also, recalculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, Up));
		Up = glm::normalize(up);
	}

	/**************************************************************************/
	/*!
	\brief  Computes the camera projection matrix based on its translation.
	\return A 4x4 matrix that is used for the shader uniform "projection".
	\note
	*/
	/**************************************************************************/
	glm::mat4 Camera::GetProjectionMatrix() {
		if (Projection == ProjectionMode::Orthographic) {

			//First parameter: left coordinate second: right coordinate third: bottom coordinate fourth: top coordinate fifth: near plane sixthL far plane
			//trace << Transform_->Translation.x << Transform_->Translation.y << "\n";

			if (Size >= 99)
			{
				Size = 99;
			}
			return glm::ortho(TransformComponent->Translation.x - ((float)1024 / 728) * (100 - Size),
				TransformComponent->Translation.x + ((float)1024 / 728) * (100 - Size),
				TransformComponent->Translation.y - ((float)728 / 1024) * (100 - Size),
				TransformComponent->Translation.y + ((float)728 / 1024) * (100 - Size),
				NearPlane,
				FarPlane);
		}
		else if (Projection == ProjectionMode::Perspective) {
			if (FieldOfView <= 1)
			{
				FieldOfView = 1;
			}
			if (FieldOfView >= 175)
			{
				FieldOfView = 175;
			}
			//trace << FieldOfView << "\n";
			return glm::perspective(FieldOfView, ((float)1024 / 728), NearPlane, FarPlane);
		}

		trace << "Camera: Wrong settings in Camera Projection settings. It should be one of the following: Orthographic or Perspective\n";
		// Throw exception here. return 0;
		return glm::mat4();

	}

	/**************************************************************************/
	/*!
	\brief Serialization functions.
	*/
	/**************************************************************************/
	void Camera::Serialize(Json::Value & root) {
		// Serialize primitives
		root["FieldOfView"] = FieldOfView;
		root["Size"] = Size;
		root["NearPlane"] = NearPlane;
		root["FarPlane"] = FarPlane;
	}

	void Camera::Deserialize(Json::Value & root) {
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
