/******************************************************************************/
/*!
\file   DebugMoveController.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class Transform;
    class RigidBody;
    class DebugMoveController : public Component {

    public:

      bool Translation = false;

      Real MoveSpeed = 10;
      Real RotSpeed = 15;
      Real JumpMultiplier = 5;

      Transform* TransformRef;
      RigidBody* RigidBodyRef;

      DebugMoveController(Entity& owner) : Component(std::string("DebugMoveController"), owner) {}
      void Initialize();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyUp* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnLogicUpdateEvent(Events::LogicUpdate* event);
      void SetFootstepSound(std::string& soundfileName);

    private:
      void PlayFootstepSound();
      void PrintTranslation();
      std::string FootstepSound;
      Boolean FootstepSoundEnabled = false;
    };

  }

}