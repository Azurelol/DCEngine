#include "InputGLFW.h"

#include "..\..\Engine\Engine.h"

namespace DCEngine {

  // Need access to the engine to get a pointer to the GLFWwindow object
  extern std::unique_ptr<Engine> Daisy; 

  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the InputGLFW class.
    */
    /**************************************************************************/
    InputGLFW::InputGLFW() {
    }

    /**************************************************************************/
    /*!
    \brief  Initialize.
    */
    /**************************************************************************/
    void InputGLFW::Initialize() {

      #if(!USE_SFML)
      _window = Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->_window;
      // Input Callback
      glfwSetKeyCallback(_window, KeyCallback);
      #endif
    }

    /**************************************************************************/
    /*!
    \brief  Update.
    */
    /**************************************************************************/
    void InputGLFW::Update(float dt) {
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void InputGLFW::Terminate() {
    }

  }

}