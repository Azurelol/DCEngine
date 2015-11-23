/******************************************************************************/
/*!
@file  DebugFade.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "DebugFade.h"
#include "EngineReference.h"
#include "Sprite.h"

namespace DCEngine {

  void DebugFade::Initialize()
  {
    SpriteComponent = Owner()->getComponent<Sprite>();
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