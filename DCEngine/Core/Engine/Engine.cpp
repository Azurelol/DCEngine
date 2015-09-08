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
    //auto a = std::mem_fn(Engine::_gameSession->GameSession::OnUpdateEvent);
   
    


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

  /**************************************************************************/
  /*!
  \brief  Connects an observer to a subject's events by passing a function pointer.
          The engine will construct the delegate which it will pass to the subject.
  \param  entity The subject to which the observer wants to listen to.
  \param  event An enum. The specific event the observer wants to listen to.
  */
  /**************************************************************************/
  //void Engine::Connect(const Entity & entity, EventType eventType, function fn) {
  //  
  //}

  //void Engine::Disconnect(const Entity & entity, EventType eventType) {
  //}

  /**************************************************************************/
  /*!
  \brief  Gets the current gamesate.
  \return A pointer to the current gamestate.
  */
  /**************************************************************************/
  GamestatePtr Engine::GetCurrentState() const {
    auto currentState = _gamestates.top();
    std::cout << "Engine::GetCurrentState - The current state is "
              << currentState->_name << std::endl;
    return currentState;
  }

  /**************************************************************************/
  /*!
  \brief  Pushes the gamestate on top of the stack.
  */
  /**************************************************************************/
  void Engine::PushGamestate(GamestatePtr gamestate) {
    std::cout << "Engine::PushGameState - " << "Gamestate: "
      << gamestate->_name << std::endl;
    _gamestates.push(gamestate);
    gamestate->Initialize();
    //CreateSpace(gamestate->_def);
    
  }

  /**************************************************************************/
  /*!
  \brief  Pops the gamestate.
  */
  /**************************************************************************/
  void Engine::PopGameState() {
    auto currentState = _gamestates.top();
    EnumeratedGamestate gamestate = currentState->GetState();
    currentState->Terminate(); // Shuts down the current state.
    _gamestates.pop(); // Next, removes it from the stack.

    // If there is a gamestate left in the stack
    if (_gamestates.size()) 
      // Activate the new gamestate
      _gamestates.top()->PoppedTo(gamestate);
    // If there's no remaining gamestates, stop the engine.
    else 
      Stop();
  }

  /**************************************************************************/
  /*!
  \brief  Creates a space with the given name.
  \param  The name of the space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr Engine::CreateSpace(std::string name) {
    std::cout << "Engine::CreateSpace - " << name << " has been created." << std::endl;

    // Check if the space already exists
    auto space = _spaces.find(name);

    // Checks if the space is a valid existing space??
    if (space != _spaces.end())
      return (*space).second;

    // If the space doesn't exist, creates it
    else
      _spaces.emplace(name, SpacePtr(new Space(name))); // See below:   
    return GetSpace(name);

    /* http://en.cppreference.com/w/cpp/container/unordered_map/emplace
    Inserts a new element into the container by constructing it in-place
    with the given args if there is no element with the key in the container.
    Careful use of emplace allows the new element to be constructed while
    avoiding unnecessary copy or move operations. The constructor of the new
    element (i.e. std::pair<const Key, T>) is called with exactly the same arguments
    as supplied to emplace, forwarded via std::forward<Args>(args)....

    If rehashing occurs due to the insertion, all iterators are invalidated.
    Otherwise iterators are not affected. References are not invalidated.
    Rehashing occurs only if the new number of elements is equal to or greater
    than max_load_factor()*bucket_count().
    */

  }

  /**************************************************************************/
  /*!
  \brief  Returns a pointer to the space.
  \param  The name of the space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr Engine::GetSpace(std::string name) {
    // Searches for a space with the specified name
    SpaceMap::iterator it = _spaces.find(name);

    // Check if the space was found
    if (it != _spaces.end())
      return it->second;

    /*
    Dereferencing a std::map iterator gives you a std::pair,
    you can then access its elements with first and second. For example, (*it).first
    will give you the key and (*it).second will give you the value.
    These are equivalent to it->first and it->second.
    */

    // Good Cop: If there was no space with that name, creates one and gives it to you.
    //return CreateSpace(name);
    // Bad Cop:  Throw a range error cuz you done goof'd
    throw std::range_error("The specified space does not exist.");
  }

  /**************************************************************************/
  /*!
  \brief  Sets the active space.
  \param  The name of the space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr Engine::SetActiveSpace(std::string name) {

    // Makes sure the space exists
    auto space = _spaces.find(name);
    if (space != _spaces.end()) {
      _activeSpace = name;
      std::cout << "Engine::SetActiveSpace - " << name 
                << " has been set as the active space." << std::endl;
      return GetActiveSpace();
    }

    // If the space didn't exist, throw the doge at the client.
    throw std::range_error("Tried to set an active space to a space that doesn't exist.");

  }

  /**************************************************************************/
  /*!
  \brief  Returns a pointer to the active space.
  \return A pointer to the space.
  */
  /**************************************************************************/
  SpacePtr Engine::GetActiveSpace() {
    auto activeSpace = _spaces.find(_activeSpace);
    return activeSpace->second;
  }


}