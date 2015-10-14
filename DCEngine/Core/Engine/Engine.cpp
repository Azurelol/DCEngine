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
#include "..\EventsInclude.h"

// Testing
#include "../Testing.h" // Dollhouse

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
    // Initialize the trace object
    using namespace Debug;
    traceObj.reset(new Trace("Log.txt"));

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
    
    trace << "[Engine::Engine - Constructor] \n";
    trace << "\n[Engine::Initialize] \n";

    throw DCException("Oh dear, something broke");

    // Autowolves, howl out!
    _active = true;

    // Construct the input interface objects
    keyboard_.reset(new Keyboard());
    mouse_.reset(new Mouse());

    // Systems are added to to the engine's systems vector. 
    _systems.push_back(SystemPtr(new Systems::Content));
    _systems.push_back(SystemPtr(new Systems::Factory));
    _systems.push_back(SystemPtr(new Systems::Window));
    _systems.push_back(SystemPtr(new Systems::Input));
    _systems.push_back(SystemPtr(new Systems::Physics));
    _systems.push_back(SystemPtr(new Systems::Audio));
    _systems.push_back(SystemPtr(new Systems::Graphics));

    //trace << "\n";
    //std::string projectFilePath = "Projects/Sample/SampleProj.dcp";
    
    // Initialize all internal engine systems
    for (auto sys : _systems) {
      sys->Initialize();
    }
    
    // Initialize the project
    trace << "[Engine::Initialize - All engine systems initialized]\n";

    // Loads the project file to start up the game
    //LoadProject(std::string("Default")); // Temporarily default
  }

  /**************************************************************************/
  /*!
  \brief Loads the project, using serialization to load all the project data.
  \param The name of the project file.
  */
  /**************************************************************************/
  void Engine::LoadProject(std::string & projectFile) {

    getSystem<Systems::Content>()->LoadProjectData(projectFile);

    trace << "\n[Engine::LoadProject - Loading " << projectFile << "]\n";

    // 1. Deserialize the input file for information about the project,
    // and store that in a struct owned by the engine.

    // 2. Scan through the "Assets" folder given for game resources:
    // SpriteSources, SoundCues, Levels. Add these resources to the content
    // system hashmaps, as "Name"/"FileName" pairs.

    // 3. Scan through the "Scripts" folder for game components (scripts):
    // Add these components dynamically to the engine's components to be used
    // for gameplay.

    // 4. Scan through the "Archetypes" folder for serialized archetypes to
    // be used in the game. These are GameObjects with specific components,
    // and will be added to an "Archetype"/"FileName" map.

    // 5. Scan the archetyped "GameSession" object and use the values in its
    // 'ProjectSetup' component: DefaultSpace, DefaultLevel to load an 
    // archetyped space and load the specified default level onto it. 
    // (This level is looked for in the content's system "Levels map" container.

    // Load all resources, both defaults and project-specific
    getSystem<Systems::Content>()->LoadAllResources();


  }

  void Engine::StartProject()
  {
    // Create the gamesession object, the "game" itself,  which contains all spaces.
    gamesession_.reset(new GameSession(_projectName));
    // Deserialize from the file

    // Load the gamesession object from the archetype
    // Add components, change non-default values

    // Load the default space
    SpacePtr defaultSpace = gamesession_->CreateSpace(_defaultSpace);

    // Load a level into the space

    // !!! TESTING: Level loading
    LevelPtr dollhouse = LevelPtr(new DollHouse(*defaultSpace.get(), *gamesession_));
    defaultSpace->LoadLevel(dollhouse);


    trace << "\n[Engine::LoadProject - Finished loading " << "]\n\n";

    // Initialize the gamesession. (This will initialize its spaces,
    // and later, its gameobjects)
    gamesession_->Initialize();

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
    using Systems::Graphics;
    // Tell window management system to begin new frame
    GETSYSTEM(Window)->StartFrame();
    GETSYSTEM(Graphics)->StartFrame();

    // Construct the update event and assign it the engine's dt
    auto logicUpdateEvent = new Events::LogicUpdate();
    logicUpdateEvent->Dt = dt;
    // Dispatch the logic update event to the gamesession
    gamesession_->Dispatch<Events::LogicUpdate>(logicUpdateEvent);
    // Dispatch the logic update event to all active spaces
    for (auto space : gamesession_->_spaces)
      space.second->Dispatch<Events::LogicUpdate>(logicUpdateEvent);
    // Delete the event
    delete logicUpdateEvent;

    // Update all the sytems at the end of the frame, based on the order
    // they were added to the engine. (Or split it and do it individually?)
    for (auto system : _systems)
      system->Update(dt);

    // Tell window management system to end the frame
    GETSYSTEM(Graphics)->EndFrame();
    GETSYSTEM(Window)->EndFrame();

    if (TRACE_UPDATE)
      trace << "[Engine::Update - All systems updated.] \n";
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