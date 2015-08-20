#include "Input.h"


#include "..\..\Debug\Debug.h" // trace

namespace DCEngine {
  namespace Systems {
    Input::Input() : System(std::string("InputSystem"), EnumeratedSystem::Input) {
      
    }

    void Input::Initialize() {
      trace << "Input::Initialize \n";
    }

    void Input::Update(float dt) {
      trace << "Input::Update \n";
    }

    void Input::Terminate() {
      trace << "Input::Terminate \n";
    }

  }


}
