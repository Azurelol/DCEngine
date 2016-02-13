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

      SpriteComponent = Owner()->getComponent<Components::Sprite>();
      TransformComponent = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>();
      Connect(SpaceRef, Events::LogicUpdate, DebugActions::OnLogicUpdateEvent);
      Connect(Daisy->getKeyboard(), Events::KeyDown, DebugActions::OnKeyDownEvent);  
      DefaultColor = SpriteComponent->getColor();
    }

    void DebugActions::TestActionSequence()
    {
      ResetValues();

      DCTrace << "DebugActions::TestActionSequence: \n";
      auto seq = Actions::Sequence(Owner()->Actions);
      Actions::Delay(seq, 1.5f);
      Actions::Call(seq, &DebugActions::Boop, this);  
      Actions::Delay(seq, 1.0f);
      Actions::Call(seq, &DebugActions::Boop, this);      
      Actions::Call(seq, &DebugActions::ReportPropertyValue, this);
      Actions::Property(seq, this->TestValue, 1.0f, 1.0f, Ease::Linear);
      // Test sprite color
      auto newColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
      Actions::Property(seq, SpriteComponent->Color, newColor, 2.0f, Ease::Linear);
      Actions::Call(seq, &DebugActions::ReportPropertyValue, this);
      //Actions::Call(seq, &DebugActions::ResetValues, this);
      
      //Actions::Call(seq, &DebugActions::Boop);
    }

    void DebugActions::Boop()
    {
      DCTrace << "Boop! \n";
    }

    void DebugActions::ReportPropertyValue()
    {
      DCTrace << "TestValue is at = " << TestValue << "\n";
    }

    void DebugActions::ResetValues()
    {
      TestValue = 5;
      SpriteComponent->setColor(DefaultColor);
    }

    void DebugActions::TestRayCasting()
    {
      // Create the ray
      auto ray = Ray();
      ray.Direction = Owner()->getComponent<Components::Transform>()->getTranslation();
      ray.Origin = Vec3(0, 0, 0);
      // Cast the the ray
      auto results = SpaceRef->getComponent<Components::PhysicsSpace>()->CastRay(ray);
      

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
      if (event->Key == Keys::F) 
        TestActionSequence();
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