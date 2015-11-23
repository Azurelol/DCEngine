#pragma once
/******************************************************************************/
/*!
@file   MouseDown.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/22/2015
@brief  The mouse scroll event.
*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  // @todo Not sure whether to use it.
  enum class MouseWheelScrollDirection {
    Up,
    Down,
  };

  namespace Events {
    class MouseScroll : public Event {
    public:
      Real Delta;
      Vec2 Direction;      
    private:

    };
  }


}