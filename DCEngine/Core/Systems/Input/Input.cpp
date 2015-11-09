#include "Input.h"

#include "..\..\Engine\Engine.h" 

// SFML implementation
#if(USE_SFML)

// GLFW implementation
#else
#include "InputGLFW.h"
std::unique_ptr<DCEngine::Systems::InputGLFW> InputHandler;
#endif

namespace DCEngine {
  
  extern std::unique_ptr<Engine> Daisy;

  namespace Systems {
    Input::Input() : System(std::string("InputSystem"), EnumeratedSystem::Input) {      
      #if(USE_SFML)
      InputHandler.reset(new InputSFML());
      #else
      InputHandler.reset(new InputGLFW());
      #endif
      
    }

    void Input::Initialize() {
      if (TRACE_ON)
        DCTrace << "Input::Initialize \n";
      InputHandler->Initialize();
    }

    void Input::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Input::Update \n";
      InputHandler->Update(dt);
    }

    void Input::Terminate() {
      if (TRACE_ON)
        DCTrace << "Input::Terminate \n";
      InputHandler->Terminate();
    }
    
    void Input::ToggleInput()
    {
      InputEnabled = !InputEnabled;
    }

  }


}
