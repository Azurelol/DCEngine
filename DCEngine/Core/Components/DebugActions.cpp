/******************************************************************************/
/*!
@file   DebugActions.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/18/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DebugActions.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
  
  namespace Components
  {

    ZilchDefineType(DebugActions, "DebugActions", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(DebugActions);
    }
    void DebugActions::Initialize() {

      TransformComponent = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>();
      Connect(SpaceRef, Events::LogicUpdate, DebugActions::OnLogicUpdateEvent);
      
      TestActionSequence();
    }

    void DebugActions::TestActionSequence()
    {
      auto seq = Actions::Sequence(Owner()->Actions);
      Actions::Delay(seq, 1.5f);
      auto& a = Owner()->Actions;
    }

    void DebugActions::DebugDraw()
    {
      auto ownerName = Owner()->Name();
      auto currPos = Vec3(TransformComponent->Translation.x,
                          TransformComponent->Translation.y,
                          TransformComponent->Translation.z);
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawRectangle(currPos, Radius, Height, Color);

    }



    void DebugActions::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void DebugActions::OnKeyUpEvent(Events::KeyUp * event)
    {

    }

    void DebugActions::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void DebugActions::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      // Calculate an offset from current position and stuff??
      DebugDraw();
    }


  }
}