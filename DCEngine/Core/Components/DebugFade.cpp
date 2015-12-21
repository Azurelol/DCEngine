/******************************************************************************/
/*!
\file   DebugFade.cpp
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "DebugFade.h"
#include "EngineReference.h"
#include "Sprite.h"

namespace DCEngine {
  namespace Components
  {
    void DebugFade::Initialize()
    {
      SpriteComponent = Owner()->getComponent<Components::Sprite>();
      Connect(SpaceRef, Events::LogicUpdate, DebugFade::OnLogicUpdateEvent);
    }

    void DebugFade::Serialize(Json::Value & root)
    {
    }

    void DebugFade::Deserialize(Json::Value & root)
    {
    }

    void DebugFade::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      // Decrement the alpha every update
      auto decrement = 0.001; // lol
      auto fadedColor = SpriteComponent->Color.a - decrement;
      // lol 2
      if (fadedColor < 0)
        return;

      SpriteComponent->Color = Vec4(SpriteComponent->Color.x,
        SpriteComponent->Color.y,
        SpriteComponent->Color.z,
        fadedColor);
    }


  }
}