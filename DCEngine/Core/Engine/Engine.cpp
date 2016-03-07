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
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Engine.h"

#include <cassert> // Assert
#include <algorithm> // for_each

#include "..\Systems\SystemsInclude.h"
#include "Timer.h"
#include "..\Debug\Debug.h" // Trace
#include "..\EventsInclude.h"

namespace DCEngine {
  
  // A pointer to the 'ENGINE' object
  std::unique_ptr<Engine> Daisy = nullptr;

  /**************************************************************************/
  /*!
  \brief  Default constructor for the Engine object.
  */
  /**************************************************************************/
  Engine::Engine() {
    // Assert makes sure there's only one instance of the engine 
    assert(Daisy == nullptr);
    Daisy.reset(this);
    // Initialize the DCTrace object
    using namespace Debug;
    traceObj.reset(new Trace("Log.txt"));
    // Load the engine's configuration from a file
    //LoadConfigurationFiles();
  }

  /**************************************************************************/
  /*!
  @brief  Constructor for the Engine object.
  @param  configFile The name of the engine's configuration file.
  */
  /**************************************************************************/
  Engine::Engine(std::string configFile)
  {
    // Assert makes sure there's only one instance of the engine 
    assert(Daisy == nullptr);
    Daisy.reset(this);

    // Initialize the DCTrace object
    using namespace Debug;
    traceObj.reset(new Trace("Log.txt"));

    // Load the engine's configuration from a file
    LoadConfigurationFiles(configFile);
  }

  /**************************************************************************/
  /*!
  @brief  The destructor for the Engine object.
  */
  /**************************************************************************/
  Engine::~Engine() {
    
    //Terminate();
  }

  /**************************************************************************/
  /*!
  @brief Initializes the engine.
  @notes 1. Systems are added to a container in the engine.
         2. Creates the default local space, sets it as the active space.
         3. Specify which systems should be updated.
         4. All systems in the container are initialized.
  */
  /**************************************************************************/
  void Engine::Initialize() {
    
    DCTrace << "[Engine::Engine - Constructor] \n";
    DCTrace << "\n[Engine::Initialize] \n";

    // Autowolves, howl out!
    Active = true;
    // Construct the input interface objects
    KeyboardHandle.reset(new Keyboard());
    MouseHandle.reset(new Mouse());
    // Systems are added to to the engine's systems container, and configurations passed on.
    Systems.push_back(SystemPtr(new Systems::Content(EngineConfiguration->AssetPath)));
    Systems.push_back(SystemPtr(new Systems::Reflection));
    Systems.push_back(SystemPtr(new Systems::Factory));
    Systems.push_back(SystemPtr(new Systems::Window(Configurations.Graphics, EngineConfiguration->Caption)));
    Systems.push_back(SystemPtr(new Systems::Input));
    // Editor configuration @todo change me next!
    Configurations.Editor.EditorEnabled = EngineConfiguration->EditorEnabled;
    Configurations.Editor.ProjectsPath = EngineConfiguration->ProjectsPath;
    Configurations.Editor.RecentProject = EngineConfiguration->RecentProject;
    // Add the systems to the engine's systems container
    Systems.push_back(SystemPtr(new Systems::Editor(Configurations.Editor)));
    Systems.push_back(SystemPtr(new Systems::Physics));
    Systems.push_back(SystemPtr(new Systems::Audio(Configurations.Audio)));
    Systems.push_back(SystemPtr(new Systems::Graphics(Configurations.Graphics)));
    Systems.push_back(SystemPtr(new Systems::GUI(Configurations.GUI)));        
    // Create the default gamesession object, the "game" itself,  which contains all spaces.
    CurrentGameSession.reset(new GameSession(_projectName));
    // Load the default space to start with
    LoadDefaultSpace();
    // Initialize all internal engine systems
    for (auto sys : Systems) {
      sys->Initialize();
    }  
    // Load all resources, both defaults and project-specific
    getSystem<Systems::Content>()->LoadAllResources();
    // Subscribe to events
    Subscribe();
    DCTrace << "[Engine::Initialize - All engine systems initialized]\n";
    // Initialize the gamesession. (This will initialize its spaces,
    // and later, its gameobjects)
    CurrentGameSession->Initialize();      

    // Open the last known recent project
    getSystem<Systems::Editor>()->OpenRecentProject();    
    Systems::DispatchSystemEvents::EngineInitialized();
  }

  /**************************************************************************/
  /*!
  @brief  Subscribes to engine-specific events.
  */
  /**************************************************************************/
  void Engine::Subscribe()
  {
    Connect<Events::WindowLostFocus>(&Engine::OnWindowLostFocusEvent, this);
    Connect<Events::WindowGainedFocus>(&Engine::OnWindowGainedFocusEvent, this);
    Connect<Events::EnginePause>(&Engine::OnEnginePauseEvent, this);
    Connect<Events::EngineResume>(&Engine::OnEngineResumeEvent, this);
    Connect<Events::EngineExit>(&Engine::OnEngineExitEvent, this);
    Connect<Events::EnginePauseMenu>(&Engine::OnEnginePauseMenuEvent, this);
    Connect<Events::EngineSaveConfigurations>(&Engine::OnEngineSaveConfigurationsEvent, this);
  }

  void Engine::OnWindowLostFocusEvent(Events::WindowLostFocus * event)
  {
    Systems::DispatchSystemEvents::EnginePause();
    DispatchGameEvents::GameFocusOut();
    
  }

  void Engine::OnWindowGainedFocusEvent(Events::WindowGainedFocus * event)
  {
    Systems::DispatchSystemEvents::EngineResume();
    DispatchGameEvents::GameFocusIn();
  }

  /**************************************************************************/
  /*!
  @brief  Pauses the engine.
  */
  /**************************************************************************/
  void Engine::OnEnginePauseEvent(Events::EnginePause * event)
  {
    DCTrace << "Engine::OnEnginePauseEvent - Paused \n";
    this->Paused = true;
  }

  /**************************************************************************/
  /*!
  @brief  Resumes the engine.
  */
  /**************************************************************************/
  void Engine::OnEngineResumeEvent(Events::EngineResume * event)
  {
    DCTrace << "Engine::OnEngineResumeEvent - Resumed \n";
    this->Paused = false;
  }

  /**************************************************************************/
  /*!
  @brief  Exits the engine.
  */
  /**************************************************************************/
  void Engine::OnEngineExitEvent(Events::EngineExit * event)
  {
    DCTrace << "Engine::OnEngineExitEvent - Exit \n";
  }

  // Deprecate me!!
  void Engine::OnEnginePauseMenuEvent(Events::EnginePauseMenu * event)
  {
    //PauseMenuEnabled = true;    
  }

  void Engine::OnEngineSaveConfigurationsEvent(Events::EngineSaveConfigurations * event)
  {
    DCTrace << "Engine::OnEngineSaveConfigurationsEvent - Saving configurations... \n";
    Configurations.Editor.Save(Systems::EditorConfig::FileName());
    Configurations.Graphics.Save(Systems::GraphicsConfig::FileName());
    Configurations.Audio.Save(Systems::AudioConfig::FileName());
    Configurations.GUI.Save(Systems::GUIConfig::FileName());
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
      DCTrace << "\n[Engine::Update] \n";
    // The profile gets updated at the end of the frame so that
    // it clears the list.
    Profile.Update(dt);

    // Tell window management system to begin new frame
    getSystem<Systems::Window>()->StartFrame();
    getSystem<Systems::Graphics>()->StartFrame();
    getSystem<Systems::GUI>()->StartFrame();

    // Dispatch the update events
    DispatchUpdateEvents(dt);
    // Update all actions
    UpdateActions(dt);

    // Update all the sytems at the end of the frame, based on the order
    // they were added to the engine. (Or split it and do it individually?)
    for (auto system : Systems) {
      system->Update(dt);
    }

    // Tell window management system to end the frame
    getSystem<Systems::Graphics>()->EndFrame();
    getSystem<Systems::GUI>()->Render();    
    getSystem<Systems::Window>()->EndFrame();
    

    if (TRACE_UPDATE)
      DCTrace << "[Engine::Update - All systems updated.] \n";
  }

  /**************************************************************************/
  /*!
  @brief  Dispatches the 'LogicUpdate' event to the gamesession and all 
          its spaces.
  */
  /**************************************************************************/
  void Engine::DispatchUpdateEvents(float dt)
  {
    //if (Paused)
    //  return;

    // Construct the update event and assign it the engine's dt
    auto logicUpdateEvent = new Events::LogicUpdate();
    auto frameUpdateEvent = new Events::FrameUpdate();
    logicUpdateEvent->Dt = dt;
    frameUpdateEvent->Dt = dt;
    // Dispatch the logic update event to the gamesession
    CurrentGameSession->Dispatch<Events::LogicUpdate>(logicUpdateEvent);
    CurrentGameSession->Dispatch<Events::FrameUpdate>(frameUpdateEvent);
    // Dispatch update events to all active spaces
    for (auto space : CurrentGameSession->ActiveSpaces) {
      space.second->Update(dt);
      // Always dispatch the 'FrameUpdate' event
      space.second->Dispatch<Events::FrameUpdate>(frameUpdateEvent);
      // Do not dispatch the 'LogicUpdate' event if the space is paused or the engine is paused
      if (space.second->getComponent<Components::TimeSpace>()->getPaused() || Paused)
        continue;
      space.second->Dispatch<Events::LogicUpdate>(logicUpdateEvent);
    }      
    // Delete the event
    delete logicUpdateEvent;
    delete frameUpdateEvent;

  }

  /**************************************************************************/
  /*!
  @brief Updates the action space, in turn updating all its active actions.
  @param dt The delta time.
  */
  /**************************************************************************/
  void Engine::UpdateActions(float dt)
  {
    if (this->Paused)
      return;

    this->ActionSpace.Update(dt);
  }

  /**************************************************************************/
  /*!
  @brief Deserializes the engine's configuration files.
  */
  /**************************************************************************/
  void Engine::LoadConfigurationFiles(const std::string& configFile)
  {
    // Load the engine's configuration from a file
    EngineConfiguration.reset(new EngineConfig);
    std::string configString;
    if (FileSystem::FileReadToString(configFile, configString))
      Serialization::Deserialize(EngineConfiguration.get(), configString);
    else
      DCTrace << "Engine::LoadConfigurationFiles - Failed to deserialize engine configuration! \n";

    // Load the Graphics Config
    LoadConfiguration(Configurations.Graphics, Systems::GraphicsConfig::FileName());
    // Load the Audio Config
    LoadConfiguration(Configurations.Audio, Systems::AudioConfig::FileName());
    // Load the Debug Config
    LoadConfiguration(Configurations.Debug, Systems::DebugConfig::FileName());
    // Load the GUI Config
    LoadConfiguration(Configurations.GUI, Systems::GUIConfig::FileName());
    // Load the Editor Config
    LoadConfiguration(Configurations.Editor, Systems::EditorConfig::FileName());

  }

  /**************************************************************************/
  /*!
  @brief Registers this Action to the engine's ActioNSpace.
  @param A reference to the action.
  */
  /**************************************************************************/
  void Engine::Register(ActionPtr action)
  {
    this->ActionSpace.Add(action);
  }

  /**************************************************************************/
  /*!
  @brief Deregisters this Action from the engine's ActionSpace.
  @param A reference to the action.
  */
  /**************************************************************************/
  void Engine::Deregister(ActionPtr action)
  {
    this->ActionSpace.Remove(action);
  }
  
  /**************************************************************************/
  /*!
  \brief Loads the project, using serialization to load all the project data.
  \param The name of the project file.
  */
  /**************************************************************************/
  void Engine::LoadProject(std::string & projectFile) {

    getSystem<Systems::Content>()->LoadProjectData(projectFile);

    DCTrace << "\n[Engine::LoadProject - Loading " << projectFile << "]\n";

    // Load all resources, both defaults and project-specific
    getSystem<Systems::Content>()->LoadAllResources();
  }

  void Engine::StartProject()
  {
    // Create the default gamesession object, the "game" itself,  which contains all spaces.
    CurrentGameSession.reset(new GameSession(_projectName));
    // Create the default space
    SpacePtr defaultSpace = CurrentGameSession->CreateSpace(_defaultSpace);
    // Set a reference to it in the GameSession object
    CurrentGameSession->DefaultSpace = defaultSpace;
    DCTrace << "\n[Engine::LoadProject - Finished loading " << "]\n\n";

    // Initialize the gamesession. (This will initialize its spaces,
    // and later, its gameobjects)
    CurrentGameSession->Initialize();

  }
  
  /**************************************************************************/
  /*!
  @brief  Loads the GameSession's default space
  */
  /**************************************************************************/
  void Engine::LoadDefaultSpace()
  {
    // Create the default space
    SpacePtr defaultSpace = CurrentGameSession->CreateSpace(_defaultSpace, false);
    // Set a reference to it in the GameSession object
    CurrentGameSession->DefaultSpace = defaultSpace;
  }

  /**************************************************************************/
  /*!
  \brief  Terminates all the systems, clears out the spaces, and shuts down
  and pops all the game states.
  */
  /**************************************************************************/
  void Engine::Terminate() {

    // Release
    MouseHandle.release();
    KeyboardHandle.release();
    //CurrentGameSession.release();

    //return;

    DCTrace << "\n[Engine::Terminate] \n";
    // Clear every Space
    for (auto space : CurrentGameSession->ActiveSpaces) {
      space.second->DestroyAll();      
    }
    // Destroy every GameObject
    getSystem<Systems::Factory>()->ActiveGameObjects.clear();
    // Clear the GameSession
    CurrentGameSession->ActiveSpaces.clear();
    CurrentGameSession.release();

    // Terminates all systems.
    for (auto sys : Systems)
      sys->Terminate();
    Systems.clear();

    Daisy.reset();
  }

  /**************************************************************************/
  /*!
  \brief  Creates a timer and calls Update(dt) repeatedly.
  */
  /**************************************************************************/
  void Engine::Loop() {
    DeltaTime = 1.0f / EngineConfiguration->Framerate;

    while (Active) {
      Time::ScopeTimer frameTimer(&DeltaTime);
      Update(DeltaTime);
    }
  }
}