/******************************************************************************/
/*!
\file   DebugReport.cpp
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DebugReport.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
  namespace Components
  {

    void DebugReport::Initialize() {

      TransformComponent = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>();
      Connect(SpaceRef, Events::LogicUpdate, DebugReport::OnLogicUpdateEvent);
    }

    void DebugReport::Serialize(Json::Value & root)
    {
    }

    void DebugReport::Deserialize(Json::Value & root)
    {
    }

    void DebugReport::OnKeyDownEvent(Events::KeyDown * event)
    {
      if (ReportTranslation) {
        DCTrace << Owner()->Name() << " is now at (" << TransformComponent->Translation.x << ", "
          << TransformComponent->Translation.y << ", "
          << TransformComponent->Translation.z << ")\n";
      }
    }

    void DebugReport::OnKeyUpEvent(Events::KeyUp * event)
    {

    }

    void DebugReport::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void DebugReport::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      // Example zilch code from my GAM150 project for drawing. example to find positions??
      /*
      // Casts a sphere of radius
          //var currPos =  this.Owner.Transform.WorldTranslation;
          var lookDir = this.Owner.Orientation.WorldForward;
          //JDebug("World Forward: `lookDir`");
          this.SphereOrigin = (this.Owner.Transform.Translation + lookDir * this.InteractRadius );
          //this.SphereOrigin = (this.Owner.Transform.Translation - lookDir * this.InteractRadius );

      */
      // Calculate an offset from current position and stuff??
      DebugDraw();
    }

    void DebugReport::DebugDraw()
    {
      auto ownerName = Owner()->Name();
      auto currPos = Vec3(TransformComponent->Translation.x,
        TransformComponent->Translation.y,
        TransformComponent->Translation.z);

      if (DrawType == DebugDrawType::Line) {
        SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(currPos, currPos + Offset, Color);
      }
      else if (DrawType == DebugDrawType::Circle) {
        SpaceRef->getComponent<Components::GraphicsSpace>()->DrawCircle(currPos, Radius, Color);
      }
      else if (DrawType == DebugDrawType::Rectangle) {
        SpaceRef->getComponent<Components::GraphicsSpace>()->DrawRectangle(currPos, Radius, Height, Color);
      }
    }

  }
}