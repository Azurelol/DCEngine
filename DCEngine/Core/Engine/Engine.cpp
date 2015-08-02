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
#include "Engine.h"

#include <cassert> // Assert

#include "..\Systems\SystemsInclude.h"
#include "..\..\Libraries\Timer.h"
#include "..\Debug\Debug.h" // PrintString

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
    Debug::PrintString("Engine::Initialize");
    
    // Autowolves, howl out!
    _active = true;

    // Systems are added to to the engine's systems vector
    _systems.push_back(SystemPtr(new Systems::WindowGLFW));

    // Creates the default local space

    // Sets the space as the active space

    // Specify which systems should be updated

    // Initialize all systems
    for (auto sys : _systems) {
      sys->Initialize();
    }

  }

  /**************************************************************************/
  /*!
  \brief  Terminates all the systems, clears out the spaces, and shuts down
          and pops all the game states.
  */
  /**************************************************************************/
  void Engine::Terminate() {
    Debug::PrintString("Engine::Terminate");

    for (auto sys : _systems)
      sys->Terminate();

    _systems.clear();

    ENGINE.reset();
  }

  /**************************************************************************/
  /*!
  \brief Updates all the systems.
  */
  /**************************************************************************/
  void Engine::Update(float dt) {
    Debug::PrintString("Engine::Update");
    using Systems::WindowGLFW;
    
    // Update the window management system (window, input)
    GETSYSTEM(WindowGLFW)->Update(dt);

    // Tell window management system to begin new frame

    // Tell window management system to end the frame
    
  }

  /**************************************************************************/
  /*!
  \brief  The main loop for the engine.
  */
  /**************************************************************************/
  void Engine::Loop() {
    dt = 1.0f / _FrameRate;

    while (_active) {
      ScopeTimer frameTimer(&dt);
      Update(dt);
    }

  }





}