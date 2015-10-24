#include "Keyboard.h"

namespace DCEngine {

  bool Keyboard::KeyIsDown(Keys key)
  {
    switch (key) {

    case Keys::Space:
      if (KeyDown_Space)
        return true;
      else
        return false;
      break;



    default:
      break;

    }    

    return false;
  }

  bool Keyboard::KeyIsUp(Keys key)
  {
    return !(KeyIsDown(key));
  }


  void Keyboard::Initialize() {
    //Connect(this, Events::KeyPressed, Keyboard::OnKeyPressedEvent);

  }

  void Keyboard::Serialize(Json::Value & root) {
  }

  void Keyboard::Deserialize(Json::Value & root) {
  }

}