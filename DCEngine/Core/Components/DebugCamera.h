#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Camera;
  class Transform;
  class DebugCamera : public Component {
  public:

    /* Properties */
    Real MoveSpeed = 3;
    Real ZoomSpeed = 10;
    
    /* Initialize */
    DebugCamera(Entity& owner) : Component(std::string("DebugCamera"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    
    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);

  private:
    Camera* CameraComponent;
    Transform* TransformComponent;


  };


}