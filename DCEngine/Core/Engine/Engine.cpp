/******************************************************************************/
/*!
\file   Engine.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The x

Functions include:
- Name
Description here.

*/
/******************************************************************************/
#include <cassert> // Assert


#include "Engine.h"
#include "..\..\Libraries\Timer.h"
#include "..\Debug\Debug.h"

namespace DCEngine {

  // A pointer to the 'ENGINE' object
  std::unique_ptr<Engine> ENGINE = nullptr; 

  /**************************************************************************/
  /*!
  \brief  The constructor for the Engine object.
  */
  /**************************************************************************/
  Engine::Engine() {
    // Assert makes sure there's only one instance of the engine 
    assert(ENGINE == nullptr);
    ENGINE.reset(this);
  }

  /**************************************************************************/
  /*!
  \brief Initializes
  */
  /**************************************************************************/
  void Engine::Initialize() {
    DCEngine::Debug::PrintString("Daisy Chain Engine Initialized. Beep boop.");
    _Active = true;

    // Systems are added to to the engine's systems vector

    // Creates the default local space

    // Sets the space as the active space

    // Specify which systems should be updated


  }

  /**************************************************************************/
  /*!
  \brief  Terminates all the systems, clears out the spaces, and shuts down
          and pops all the game states.
  */
  /**************************************************************************/
  void Engine::Terminate() {
    DCEngine::Debug::PrintString("Terminating Daisy Chain Engine... ");
    ENGINE.reset();
  }

  /**************************************************************************/
  /*!
  \brief Updates all the systems.
  */
  /**************************************************************************/
  void Engine::Update(float dt) {
    DCEngine::Debug::PrintString("Updating!");
    DCEngine::Debug::PrintFPS(dt);
    
  }

  /**************************************************************************/
  /*!
  \brief  The main loop for the engine.
  */
  /**************************************************************************/
  void Engine::Loop() {
    dt = 1.0f / _FrameRate;

    while (_Active) {
      ScopeTimer frameTimer(&dt);
      Update(dt);
    }

  }





}