#include "DebugCollider.h"
#include "EngineReference.h"
#include "Sprite.h"

namespace DCEngine {

  void DebugCollider::Initialize()
  {
    auto gameObj = dynamic_cast<GameObject*>(Owner());
    SpriteComponent = gameObj->getComponent<Sprite>();
    //CollisionCue = SoundCue(String("CollisionSound"), String("collision.wav"));

    // Save the current color
    SpriteColor = SpriteComponent->Color;
    // Connect to collision events
    Connect(gameObj, Events::CollisionStarted, DebugCollider::OnCollisionStartedEvent);
    Connect(gameObj, Events::CollisionEnded, DebugCollider::OnCollisionEndedEvent);
  }

  void DebugCollider::Serialize(Json::Value & root)
  {
  }

  void DebugCollider::Deserialize(Json::Value & root)
  {
  }

  void DebugCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
  {
    //trace << Owner()->Name() << "::DebugCollider::OnCollisionStartedEvent - Colliding with  "
    //      << event->OtherObject->Name() << "\n";

    // Change color    
    if (ChangeColorOnCollide)
      SpriteComponent->Color = CollisionColor;
    // Play a collision 
    //SpaceRef->getComponent<SoundSpace>()->PlayCue();
  }

  void DebugCollider::OnCollisionEndedEvent(Events::CollisionEnded * event)
  {
    // Change the color back
    if (ChangeColorOnCollide)
      SpriteComponent->Color = SpriteColor;

    // Stop playing a collision
  }
}