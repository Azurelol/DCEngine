#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  enum class DebugDrawType {
    Line,
    Circle,
    Rectangle,
    None,
  };

  class Transform;
  class DebugReport : public Component {
    
  public:

    Transform* TransformComponent;
    Boolean ReportTranslation = false;
    DebugDrawType DrawType = DebugDrawType::None;
    Real Radius = 1;
    Real Height = 1;
    Real3 Offset = Real3(0, 0, 0);
    Real4 Color = Real4(0.5, 0.5, 0.5, 1);
    //Real3 StartPos = Real3(0, 0, 0);
    //Real3 EndPos = Real3(0, 0, 0);  

    DebugReport(Entity& owner) : Component(std::string("DebugReport"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void OnMouseDownEvent(Events::MouseDown* event);
    void OnLogicUpdateEvent(Events::LogicUpdate* event);

    void DebugDraw();
  };


}