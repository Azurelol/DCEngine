#include "DebugReport.h"
#include "EngineReference.h"
#include "Transform.h"

namespace DCEngine {
    
  void DebugReport::Initialize() {   
    
    TransformRef = dynamic_cast<GameObject*>(owner_)->getComponent<Transform>();
    Connect(space_, Events::LogicUpdate, DebugReport::OnLogicUpdateEvent);
  }

  void DebugReport::Serialize(Json::Value & root)
  {
  }

  void DebugReport::Deserialize(Json::Value & root)
  {
  }

  void DebugReport::OnKeyDownEvent(Events::KeyDown * event)
  {
    // Keyboard Input broken?
    //switch (event->Key) {
    //case Keys::W:
      trace << Owner()->Name() << " is now at (" << TransformRef->Translation.x << ", "
                                                 << TransformRef->Translation.y << ", "
                                                 << TransformRef->Translation.z << ")\n";
    //  break;    
    //}
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


    
    auto pos = Real3(1, 1, 1);
    auto radius = 5.0f;
    auto color = Real4(1, 0, 1, 1);
    space_->getComponent<GraphicsSpace>()->DrawCircle(pos, radius, color);
	//space_->getComponent<GraphicsSpace>()->DrawLineSegment(pos, Real3(5, 1, 1), radius, color);
	//space_->getComponent<GraphicsSpace>()->DrawRectangle(pos, radius, radius, color);
    //trace << "hey";
   
  }

}