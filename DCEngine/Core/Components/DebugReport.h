#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class DebugReport : public Component {

  public:

    Transform* TransformRef;

    DebugReport(Entity& owner) : Component(std::string("DebugReport"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void OnMouseDownEvent(Events::MouseDown* event);
    void OnLogicUpdateEvent(Events::LogicUpdate* event);
  };


}