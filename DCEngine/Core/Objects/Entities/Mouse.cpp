/******************************************************************************/
/*!
\file   Mouse.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The mouse input inteface for the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/

#include "Mouse.h"

namespace DCEngine {

  bool Mouse::MouseDown(MouseButton button)
  {
    switch (button) {

    case MouseButton::Left:
      if (MouseDown_Left)
        return true;
      else
        return false;
      break;

    case MouseButton::Right:
      if (MouseDown_Right)
        return true;
      else
        return false;
      break;

    case MouseButton::Middle:
      if (MouseDown_Middle)
        return true;
      else
        return false;
      break;


    default:
      break;
    }

    return false;

  }
  bool Mouse::MouseUp(MouseButton button)
  {
    // Why double the amount of code when I can just
    // return the opposite of MouseDown?
    // HOHOHOHO
    return !MouseDown(button);
  }

  void Mouse::Initialize() {
  }

}