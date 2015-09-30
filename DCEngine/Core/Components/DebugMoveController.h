#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class DebugMoveController : public Component {

  public:
        
    bool Translation = true;
    Real MoveSpeed = 0.75;
    Real RotSpeed = 15;
    Transform* TransformRef; 

    DebugMoveController(Entity& owner) : Component(std::string("DebugMoveController"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void OnMouseDownEvent(Events::MouseDown* event);
    void SetFootstepSound(std::string& soundfileName);

  private:
    void PlayFootstepSound();
    std::string FootstepSound;
    Boolean FootstepSoundEnabled = false;
  };


}