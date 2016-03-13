/******************************************************************************/
/*!
\file   DebugCollider.cpp
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DebugCollider.h"
#include "EngineReference.h"
#include "Sprite.h"

namespace DCEngine {
  namespace Components
  {

    void DebugCollider::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      SpriteComponent = gameObj->getComponent<Components::Sprite>();
      //CollisionCue = SoundCue(String("CollisionSound"), String("collision.wav"));

      // Save the current color
      SpriteColor = SpriteComponent->Color;
      // Connect to collision events
      Connect(gameObj, Events::CollisionStarted, DebugCollider::OnCollisionStartedEvent);
      Connect(gameObj, Events::CollisionEnded, DebugCollider::OnCollisionEndedEvent);
    }
    
    void DebugCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      //DCTrace << Owner()->Name() << "::DebugCollider::OnCollisionStartedEvent - Colliding with  "
      //      << event->OtherObject->Name() << "\n";

      // Change color    
      if (ChangeColorOnCollide)
        SpriteComponent->Color = CollisionColor;
      // Play a collision 
      //SpaceRef->getComponent<Components::SoundSpace>()->PlayCue();
    }

    void DebugCollider::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
      // Change the color back
      if (ChangeColorOnCollide)
        SpriteComponent->Color = SpriteColor;

      // Stop playing a collision
    }
  }
}