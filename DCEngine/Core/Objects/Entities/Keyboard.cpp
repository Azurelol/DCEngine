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

	case Keys::W:
		if (KeyDown_W)
			return true;
		else
			return false;
		break;

	case Keys::A:
		if (KeyDown_A)
			return true;
		else
			return false;
		break;

	case Keys::S:
		if (KeyDown_S)
			return true;
		else
			return false;
		break;

	case Keys::D:
		if (KeyDown_D)
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