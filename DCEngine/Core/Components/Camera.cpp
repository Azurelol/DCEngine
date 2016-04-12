/******************************************************************************/
/*!
@file   Camera.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/12/2015
@brief  Camera component implementation.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Camera.h"

#include "EngineReference.h"
#include "Transform.h"

/**************************************************************************/
/*!
@brief Binds the ProjectionMode enum class.
*/
/**************************************************************************/
ZilchDefineExternalType(DCEngine::ProjectionMode, "ProjectionMode", DCEngine::DCEngineCore, builder, type) {
  ZilchBindEnum(builder, type, SpecialType::Enumeration);

  ZilchBindEnumValue(builder, type, DCEngine::ProjectionMode::Orthographic, "Orthographic");
  ZilchBindEnumValue(builder, type, DCEngine::ProjectionMode::Perspective, "Perspective");
}


namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Camera, "Camera", DCEngineCore, builder, type) {      
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Camera);
      // Properties
      DCE_BINDING_PROPERTY_DEFINE_RANGE(FieldOfView, 0, 180);
      DCE_BINDING_DEFINE_PROPERTY(Camera, FieldOfView);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyFieldOfView, attributeRangeFieldOfView);

			DCE_BINDING_DEFINE_PROPERTY(Camera, Exposure);
			DCE_BINDING_PROPERTY_DEFINE_RANGE(Exposure, 0, 5);
			DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyExposure, attributeRangeExposure);

      DCE_BINDING_DEFINE_PROPERTY(Camera, NearPlane);
      DCE_BINDING_DEFINE_PROPERTY(Camera, FarPlane);
      DCE_BINDING_DEFINE_PROPERTY(Camera, Size);
      DCE_BINDING_DEFINE_PROPERTY(Camera, Projection);
    }
#endif

    /**************************************************************************/
    /*!
    @brief Camera Constructor
    */
    /**************************************************************************/
    Camera::Camera(Entity & owner) : Component(std::string("Camera"), owner), 
      FieldOfView(90.0f), Size(90.0f), Yaw(90.0f), Pitch(0.0f), Roll(90.0f), Exposure(.5)
    {
      Configure();
    }

    /**************************************************************************/
    /*!
    @brief Camera destructor.
    */
    /**************************************************************************/
    Camera::~Camera()
    {
      // If the camera is active, inform the camera viewport that it's been
      // destroyed.
      if (Active) {
        auto currentCam = SpaceRef->getComponent<Components::CameraViewport>()->getCamera();
        if (currentCam == this) {
          DCTrace << ObjectName << "::Camera::~Camera - Removing default camera from the space \n";
          SpaceRef->getComponent<Components::CameraViewport>()->setCamera(nullptr);
        }

      }

    }

    /**************************************************************************/
    /*!
    \brief Initializes the camera component and its settings.
    \note
    */
    /**************************************************************************/
    void Camera::Initialize() {    
      DCTrace << "Camera::Initialize \n";
    }

    /**************************************************************************/
    /*!
    @brief Configures the Camera settings.
    */
    /**************************************************************************/
    void Camera::Configure()
    {
      auto gameObjOwner = (GameObject*)Owner();
      TransformComponent = gameObjOwner->getComponent<Components::Transform>();

      // Camera needs a Transform component
      if (!TransformComponent)
      {
        throw DCException("Camera::Initialize - Failed. No Transform component");
        return;
      }

      // Set the references to the Window system's screen width and height
      ScreenWidth = &Daisy->getSystem<Systems::Window>()->Settings.ScreenWidth;
      ScreenHeight = &Daisy->getSystem<Systems::Window>()->Settings.ScreenHeight;
    }

    /**************************************************************************/
    /*!
    \brief The camera is updated on every LogicUpdate event.
    \param The update event.
    */
    /**************************************************************************/
    void Camera::OnLogicUpdate(Events::LogicUpdate* event)
    {
      //DCTrace << "Camera::OnLogicUpdate::Updatelol";
      //Update();
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
      //DCTrace << FieldOfView << "\n";
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
    glm::mat4 Camera::GetProjectionMatrix(){
      if (Projection == ProjectionMode::Orthographic) {

        //First parameter: left coordinate second: right coordinate third: bottom coordinate fourth: top coordinate fifth: near plane sixthL far plane
        //DCTrace << Transform_->Translation.x << Transform_->Translation.y << "\n";

        if (Size >= 99)
        {
          Size = 99;
        }
        return glm::ortho(TransformComponent->Translation.x - ((float)(*ScreenWidth) / (*ScreenHeight)) * (100 - Size),
          TransformComponent->Translation.x + ((float)(*ScreenWidth) / (*ScreenHeight)) * (100 - Size),
          TransformComponent->Translation.y - ((float)(*ScreenHeight) / (*ScreenWidth)) * (100 - Size),
          TransformComponent->Translation.y + ((float)(*ScreenHeight) / (*ScreenWidth)) * (100 - Size),
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
        //DCTrace << FieldOfView << "\n";
        return glm::perspective(FieldOfView, ((float)(*ScreenWidth) / (*ScreenHeight)), NearPlane, FarPlane);
      }


      DCTrace << "Camera: Wrong settings in Camera Projection settings. It should be one of the following: Orthographic or Perspective\n";
      // Throw exception here. return 0;
      return glm::mat4();

    }
  }

}
  
