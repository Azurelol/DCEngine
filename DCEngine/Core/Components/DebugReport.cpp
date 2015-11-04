#include "DebugReport.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
    
  void DebugReport::Initialize() {   
    
    TransformComponent = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>();
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
      trace << Owner()->Name() << " is now at (" << TransformComponent->Translation.x << ", "
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
      SpaceRef->getComponent<GraphicsSpace>()->DrawLineSegment(currPos, currPos + Offset, Radius, Color);
    }
    else if (DrawType == DebugDrawType::Circle) {
      SpaceRef->getComponent<GraphicsSpace>()->DrawCircle(currPos, Radius, Color);
    }
    else if (DrawType == DebugDrawType::Rectangle) {
      SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(currPos, Radius, Height, Color);
    }
  }

}