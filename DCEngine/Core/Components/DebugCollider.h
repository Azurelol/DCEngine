#pragma once
#include "ComponentReference.h"

namespace DCEngine{

  class Sprite;
  class BoxCollider;
  class DebugCollider : public Component {
  public:

    /* Properties */
    Boolean ChangeColorOnCollide = false;
    Real4 CollisionColor = Real4(1, 0, 0, 1);
    Real4 SpriteColor;
    //SoundCue CollisionCue;

    /* Initialize */
    DebugCollider(Entity& owner) : Component(std::string("DebugCollider"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);    
    /* Events */
    void OnCollisionStartedEvent(Events::CollisionStarted* event);
    void OnCollisionEndedEvent(Events::CollisionEnded* event);

  private:
    Sprite* SpriteComponent;

  };


}