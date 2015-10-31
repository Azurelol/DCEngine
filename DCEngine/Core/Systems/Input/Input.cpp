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
        trace << "Input::Initialize \n";
      InputHandler->Initialize();
    }

    void Input::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Input::Update \n";
      InputHandler->Update(dt);
    }

    void Input::Terminate() {
      if (TRACE_ON)
        trace << "Input::Terminate \n";
      InputHandler->Terminate();
    }

    void Input::Serialize(Json::Value & root)
    {
    }

    void Input::Deserialize(Json::Value & root)
    {
    }

    void Input::ToggleInput()
    {
      InputEnabled = !InputEnabled;
    }

  }


}
