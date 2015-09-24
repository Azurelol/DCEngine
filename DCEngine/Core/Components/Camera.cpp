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
    Transform_ = Owner()->getComponent<Transform>();

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



  /**************************************************************************/
  /*!
  \brief Updates the camera vectors.
  \note
  */
  /**************************************************************************/
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

  /**************************************************************************/
  /*!
  \brief Calculates the view matrix off the camera and returns it.
  \return The view matrix.
  \note
  */
  /**************************************************************************/
  glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Transform_->Translation, Transform_->Translation + Front, Up);
  }
  
  /**************************************************************************/
  /*!
  \brief Calculates the current projection matrix and returns it.
  \return The projection matrix (Orthographic or Perspective)
  \note
  */
  /**************************************************************************/
  glm::mat4 Camera::GetProjectionMatrix() {
    glm::mat4 projectionMat;
    // Calculate and set the desired projection
    switch (Projection) {
    case ProjectionMode::Orthographic:
      projectionMat = glm::ortho(Transform_->Translation.x - Transform_->Scale.x * .5f,
                                       Transform_->Translation.x + Transform_->Scale.x * .5f,
                                       Transform_->Translation.y - Transform_->Scale.y * .5f,
                                       Transform_->Translation.y + Transform_->Scale.y * .5f,
                                       NearPlane, FarPlane);
      
      break;
    case ProjectionMode::Perspective:
      projectionMat = glm::perspectiveFov(FieldOfView,
                                                      Transform_->Scale.x,
                                                      Transform_->Scale.y,
                                                      NearPlane,
                                                      FarPlane);
      
      break;
    }

    return projectionMat;    

    //if (Projection == ProjectionMode::Orthographic) {
    //  //First parameter: left coordinate second: right coordinate third: bottom coordinate fourth: top coordinate fifth: near plane sixthL far plane
    //  return glm::ortho(Size * 40 / 2, Transform_->Translation.x + Size * 40 / 2, Size * 40 / 2, Size * 40 / 2, NearPlane, FarPlane);
    //}
    //else if (Projection == ProjectionMode::Perspective) {
    //  return glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    //}
    //else

    //{
    //  trace << "Camera: Wrong settings in Camera Projection settings. It should be one of the following: Orthographic or Perspective\n";
    //}
  }

}
