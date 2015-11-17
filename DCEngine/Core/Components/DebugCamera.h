#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Camera;
  class Transform;
  class DebugCamera : public Component {
  public:

    /* Properties */
    Real MoveSpeed = 3;
    Real RotSpeed = 15;
    Real ZoomSpeed = 10;
    
    /* Initialize */
    DebugCamera(Entity& owner) : Component(std::string("DebugCamera"), owner) {}
    void Initialize();
    /* Events */
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyUp* event);
    void PrintTranslation();

  private:
    Camera* CameraComponent;
    Transform* TransformComponent;
  };
}