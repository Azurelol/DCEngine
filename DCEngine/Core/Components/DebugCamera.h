/******************************************************************************/
/*!
\file   Camera.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/15/2015
\brief  The debugcamera class.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Camera;
  class Transform;
  class DebugCamera : public Component {
  public:

    /* Properties */
    Real MoveSpeed = 3;
    Real RotSpeed = 15;
    Real ZoomSpeed = 10;
    
    /* Initialize */
    DebugCamera(Entity& owner) : Component(std::string("DebugCamera"), owner) {}
    void Initialize();
    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void PrintTranslation();

  private:
    Camera* CameraComponent;
    Transform* TransformComponent;
  };
}