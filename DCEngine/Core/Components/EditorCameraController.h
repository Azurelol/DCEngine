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

  class Camera;
  class Transform;
  class EditorCameraController : public Component {
  public:

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(EditorCameraController, Component);
    #endif

    /* Properties */
    Real MoveSpeed = 3;
    Real RotSpeed = 15;
    Real ZoomSpeed = 10;

    /* Initialize */
    EditorCameraController(Entity& owner) : Component(std::string("EditorCameraController"), owner) {}
    void Initialize();
    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void OnMouseScrollEvent(Events::MouseScroll* event);
    

  private:
    Camera* CameraComponent;
    Transform* TransformComponent;
  };
}