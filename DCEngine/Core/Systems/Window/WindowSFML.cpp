#include <iostream>

#include "WindowSFML.h"
#include "..\..\Engine\Engine.h"
#include "..\..\Debug\Debug.h"

const auto ScreenWidth = 1280;
const auto ScreenHeight = 720;

namespace DCEngine {

  // A pointer to the Engine object
  extern std::unique_ptr<Engine> ENGINE;

  namespace Systems {
    WindowSFML::WindowSFML() : System(std::string("WindowsSFMLSystem"), ES_WindowSFML),
                               _Fullscreen(false),
                               _Window(nullptr),
                               _Context(),
                               _Width(ScreenWidth),
                               _Height(ScreenHeight)
    {
      DCEngine::Debug::PrintString("WindowSFML constructed"); 
    }

    void WindowSFML::Initialize() {
      // Register the component

      // Tell SFML to initialize

      // Create the screen, Set position and screen size

      // Create the context for OpenGL
    }

    void WindowSFML::Update(float dt) {
    }

    void WindowSFML::Terminate() {
    }

  }


}

