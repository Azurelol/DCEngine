/******************************************************************************/
/*!
@file   DebugActions.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/18/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  
  namespace Components
  {
    class Transform;
    class DebugActions : public Component {

    public:
      ZilchDeclareDerivedType(DebugActions, Component);
      DebugActions(Entity& owner) : Component(std::string("DebugActions"), owner) {}
      void Initialize();
      // Events
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyUp* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnLogicUpdateEvent(Events::LogicUpdate* event);
      // Tests
      void TestActionSequence();
      void DebugDraw();

    private:
      Real Radius = 1;
      Real Height = 1;
      Vec3 Offset = Vec3(0, 0, 0);
      Vec4 Color = Vec4(0.5, 0.5, 0.5, 1);
      Transform* TransformComponent;

    };


  }
}