/******************************************************************************/
/*!
@file   EditorCameraController.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/22/2015
@brief  The EditorCameraController...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class Camera;
    class Transform;
    class EditorCameraController : public Component {
    public:
      
      // Properties
      Real MoveSpeed = 3.0f;
      Real RotSpeed = 15.0f;
      Real ZoomSpeed = 5.0f;
      Real ZoomRatio = 0.08f;
	  Real ZoomMin;
	  Real ZoomMax;

      Boolean MoveByKey = false;
      DCE_DEFINE_PROPERTY(Real, MoveSpeed);
      DCE_DEFINE_PROPERTY(Real, RotSpeed);
      DCE_DEFINE_PROPERTY(Real, ZoomSpeed);
	  DCE_DEFINE_PROPERTY(Real, ZoomMax);
      // Initialize
      ZilchDeclareDerivedType(EditorCameraController, Component);
      EditorCameraController(Entity& owner);
      void Initialize();
      // Events 
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyUp* event);
      void OnMouseScrollEvent(Events::MouseScroll* event);

    private:
      Camera* CameraComponent;
      Transform* TransformComponent;
    };
  }
}