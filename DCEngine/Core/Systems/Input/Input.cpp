/******************************************************************************/
/*!
@file   Input.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/17/2015
@brief  The abstract interface for the Input class.
@todo   Find another way to pass the event object to the GUI?
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
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
  
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Input System constructor.    
    */
    /**************************************************************************/
    Input::Input() : System(std::string("InputSystem"), EnumeratedSystem::Input) {      
      #if(USE_SFML)
      InputHandler.reset(new InputSFML());
      #else
      InputHandler.reset(new InputGLFW());
      #endif
      
    }

    /**************************************************************************/
    /*!
    @brief Initializes the Input system.
    */
    /**************************************************************************/
    void Input::Initialize() {
      if (TRACE_ON)
        DCTrace << "Input::Initialize \n";
      InputHandler->Initialize();
      // Subscribe to events
      Subscribe();
    }

    /**************************************************************************/
    /*!
    @brief Subscribes the Input system to events.
    */
    /**************************************************************************/
    void Input::Subscribe()
    {
      Daisy->Connect<Events::EnginePause>(&Input::OnEnginePauseEvent, this);
      Daisy->Connect<Events::EngineResume>(&Input::OnEngineResumeEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Pauses the Input system, preventing all input events.
    */
    /**************************************************************************/
    void Input::OnEnginePauseEvent(Events::EnginePause * event)
    {
      if (DCE_TRACE_PAUSE)
        DCTrace << "Input::OnEnginePausedEvent - Pause \n";
      this->Paused = true;
      this->InputHandler->Paused = true;
    }

    /**************************************************************************/
    /*!
    @brief Resumes the Input system, resuming all input events.
    */
    /**************************************************************************/
    void Input::OnEngineResumeEvent(Events::EngineResume * event)
    {
      if (DCE_TRACE_PAUSE)
        DCTrace << "Input::OnEngineResumeEvent - Resume \n";
      this->Paused = false;
      this->InputHandler->Paused = false;
    }

    /**************************************************************************/
    /*!
    @brief Updates the Input system.
    @param dt The delta time.
    */
    /**************************************************************************/
    void Input::Update(float dt) {
      SystemTimer profile(this->Name());
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Input::Update \n";
      InputHandler->Update(dt);

    }

    /**************************************************************************/
    /*!
    @brief Terminates the Input system.
    */
    /**************************************************************************/
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
