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

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Camera, "Camera", DCEngineCore, builder, type) {
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Camera, "owner", Entity&);
    ZilchBindDestructor(builder, type, Camera);
    // Properties
    //DCE_BINDING_DEFINE_PROPERTY(Camera, Projection);
    ZilchBindProperty(builder, type, &Camera::getFieldOfView, &Camera::setFieldOfView, "FieldOfView");
    ZilchBindProperty(builder, type, &Camera::getNearPlane, &Camera::setNearPlane, "NearPlane");
    ZilchBindProperty(builder, type, &Camera::getFarPlane, &Camera::setFarPlane, "FarPlane");
    ZilchBindProperty(builder, type, &Camera::getSize, &Camera::setSize, "Size");
  }
  #endif

  /**************************************************************************/
  /*!
  @brief Camera Constructor
  */
  /**************************************************************************/
  Camera::Camera(Entity & owner) : Component(std::string("Camera"), owner) {
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
      auto currentCam = SpaceRef->getComponent<CameraViewport>()->getCamera();
      if (currentCam == this) {
        DCTrace << ObjectName << "::Camera::~Camera - Removing default camera from the space \n";
        SpaceRef->getComponent<CameraViewport>()->setCamera(nullptr);
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

  }

  /**************************************************************************/
  /*!
  @brief Configures the Camera settings.
  */
  /**************************************************************************/
  void Camera::Configure()
  {
    auto gameObjOwner = (GameObject*)Owner();
    TransformComponent = gameObjOwner->getComponent<Transform>();

    // Camera needs a Transform component
    if (!TransformComponent)
    {
      throw DCException("Camera::Initialize - Failed. No Transform component");
      return;
    }

    // Set the references to the Window system's screen width and height
    ScreenWidth = &Daisy->getSystem<Systems::Window>()->Width;
    ScreenHeight = &Daisy->getSystem<Systems::Window>()->Height;
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
  glm::mat4 Camera::GetProjectionMatrix() {
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

  /**************************************************************************/
  /**************************************************************************!
                                     PROPERTIES

  /**************************************************************************/
  Real Camera::getFieldOfView() const
  {
    return this->FieldOfView;
  }

  void Camera::setFieldOfView(Real fov)
  {
    this->FieldOfView = fov;
  }

  Real Camera::getNearPlane() const
  {
    return this->NearPlane;
  }

  void Camera::setNearPlane(Real plane)
  {
    this->NearPlane = plane;
  }

  Real Camera::getFarPlane() const
  {
    return this->FarPlane;
  }

  void Camera::setFarPlane(Real plane)
  {
    this->FarPlane = plane;
  }

  Real Camera::getSize() const
  {
    return this->Size;
  }

  void Camera::setSize(Real size)
  {
    this->Size = size;
  }

}
  
