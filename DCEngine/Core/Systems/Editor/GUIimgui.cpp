#include "GUIimgui.h"

// Window Library
#include <SFML\Window.hpp>
// Access to the Window System
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    GUI::GUI()
    {
    }

    void GUI::Initialize()
    {      
      if (TRACE_INITIALIZE)
        trace << "GUI::Initialize \n";
    }

    void GUI::Update(float dt)
    {
      // 
    }

    void GUI::Terminate()
    {
    }

  }
}