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
  void Mouse::Serialize(Json::Value & root) {
  }
  void Mouse::Deserialize(Json::Value & root) {
  }
}