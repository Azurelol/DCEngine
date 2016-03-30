/******************************************************************************/
/*!
\file   Debugfade.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {  
  namespace Components
  {
    class Sprite;
    class DebugFade : public Component {
    public:

      Sprite* SpriteComponent;
      Real FadeOutDur = 10;
      Vec4 FadeColor = Vec4(0, 0, 0, 0);

      /* Initialize */
      DebugFade(Entity& owner) : Component(std::string("DebugFade"), owner) {}
      void Initialize();

      /* Events */
      void OnLogicUpdateEvent(Events::LogicUpdate* event);

    private:
    };
  }
}