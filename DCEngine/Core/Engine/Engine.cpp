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
#include <algorithm> // for_each

#include "..\Systems\SystemsInclude.h"
#include "..\..\Libraries\Timer.h"
#include "..\Debug\Debug.h" // Trace

#include "..\Config.h" // DefaultSpace

namespace DCEngine {

  // A pointer to the 'ENGINE' object
  std::unique_ptr<Engine> Daisy = nullptr;  

  /**************************************************************************/
  /*!
  \brief  The constructor for the Engine object.
  */
  /**************************************************************************/
  Engine::Engine() {
    // Assert makes sure there's only one instance of the engine 
    assert(Daisy == nullptr);
    Daisy.reset(this);
    
  }
  
  /**************************************************************************/
  /*!
  \brief  The destructor for the Engine object.
  */
  /**************************************************************************/
  Engine::~Engine() {
    //Terminate();
  }

  /**************************************************************************/
  /*!
  \brief Initializes the engine.
        1. Systems are added to a container in the engine.
        2. Creates the default local space, sets it as the active space.
        3. Specify which systems should be updated.
        4. All systems in the container are initialized.
  */
  /**************************************************************************/
  void Engine::Initialize() {
    
    // Initialize the trace object
    using namespace Debug;
    traceObj.reset(new Trace("Log.txt"));  
    
    trace << "[Engine::Engine - Constructor] \n";
    trace << "\n[Engine::Initialize] \n";

    // Autowolves, howl out!
    _active = true;

    // Systems are added to to the engine's systems vector. 
    // (?) Why should the engine have the same systems as the spaces?
    _systems.push_back(SystemPtr(new Systems::Window));
    _systems.push_back(SystemPtr(new Systems::Input));
    _systems.push_back(SystemPtr(new Systems::GraphicsGL));
    _systems.push_back(SystemPtr(new Systems::Audio));

    // Initialize all internal engine systems
    for (auto sys : _systems) {
      sys->Initialize();
    }
    trace << "[Engine::Initialize - All engine systems initialized]\n";

    // Create the gamesession object, the "game" itself,  which contains all spaces.
    _gameSession.reset(new GameSession(_projectName));
    
    // Connect it to engine update events
    
    // Create a downcasted entity reference to pass into Connect()
    Entity& gamessionRef = *_gameSession; // <- UGLY

    //gsFunction onUpdate = &(Engine::_gameSession)->GameSession::OnUpdateEvent;
    //Connect(gamessionRef, EventType::UpdateEvent, onUpdate);

    auto b = std::mem_fn(&Engine::Update);
    auto a = std::mem_fn(&GameSession::OnUpdateEvent);
   

    //Connect(_gameSession, updateEvent, _gameSession->OnUpdateEvent)
    //Connect(_gameSession, , )

    // Initialize it
    _gameSession->Initialize();

  }

  /**************************************************************************/
  /*!
  \brief Updates the engine, at multiple levels. 
         1. The window handler is updated, sending events about window changes 
         and input events.
         2. The gamestate's every space that the engine decides is updated.
         When a space is updated, it provides each of the systems added to it
         with a vector of entities that meet the system's registration
         requirements.
         The space then tells each system to update.
  \param The time that elapsed during the last frame update.

  */
  /**************************************************************************/
  void Engine::Update(float dt) {
    if (TRACE_UPDATE)
      trace << "\n[Engine::Update] \n";
    
    using Systems::Window;
    // Update the window management system (window, input)
    GETSYSTEM(Window)->Update(dt);

    // Tell window management system to begin new frame
    GETSYSTEM(Window)->StartFrame();    

    // Update the current GameSession, which will propagate the update
    // through all its spaces, and the spaces into all objects in the game
    _gameSession->Update(dt);

    // Need to create the event object here to pass it.
    //UpdateEvent updateObj(); // Find a better syntax?
    //_gameSession->Dispatch(updateEvent);
    //_gameSession->DispatchLol(updateEvent);
    //_gameSession->DispatchEvent()
    
    if (TRACE_UPDATE)
      trace << "[Engine::Update - All systems updated.] \n";

    // Tell window management system to end the frame
    GETSYSTEM(Window)->EndFrame();
  }  

  /**************************************************************************/
  /*!
  \brief  Terminates all the systems, clears out the spaces, and shuts down
  and pops all the game states.
  */
  /**************************************************************************/
  void Engine::Terminate() {
    trace << "\n[Engine::Terminate] \n";

    for (auto sys : _systems)
      sys->Terminate();

    _systems.clear();

    Daisy.reset();
  }

  /**************************************************************************/
  /*!
  \brief  Creates a timer and calls Update(dt) repeatedly. 
  */
  /**************************************************************************/
  void Engine::Loop() {
    dt = 1.0f / _framerate;

    while (_active) {
      ScopeTimer frameTimer(&dt);
      Update(dt);
    }
  }




}