/******************************************************************************/
/*!
\file   KeyPressed.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  The keyboard input event for when a key is pressed.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EventReference.h"
#pragma once

namespace DCEngine {

  class GameObject;

  namespace Events {
    class SpriteRegistration : public Event {
    public:
      GameObject* SpriteObj;

    private:

    };
  }


}