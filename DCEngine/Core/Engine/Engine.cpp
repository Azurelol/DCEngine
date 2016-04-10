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


#include "Timer.h"
#include "..\EventsInclude.h"

namespace DCEngine {
  
  // A pointer to the 'ENGINE' object
  std::unique_ptr<Engine> Daisy = nullptr;


  /**************************************************************************/
  /*!
  @brief Connects a ZilchComponent to an Entity.
  @param call A reference to the call object.
  @param report A reference to the report object.
  */
  /**************************************************************************/
  static void ZilchConnect(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
    auto publisher = reinterpret_cast<Entity*>(call.Get<Entity*>(0));
    auto eventType = call.Get<Zilch::String>(1);
    auto& deleg = call.GetDelegate(2);
    // Gets the pointers needed
    auto observer = reinterpret_cast<Component*>(deleg.ThisHandle.Dereference());
    // Creates the delegate to be added to the entity
    auto zilchFnDeleg = new EventZilchFunctionDelegate();
    zilchFnDeleg->State = call.GetState();
    zilchFnDeleg->Delegate = deleg;

    Daisy->ConnectTo(eventType.c_str(), publisher, zilchFnDeleg, observer);
  }

  /*!************************************************************************\
  @brief  Engine, Daisy, Definition
  \**************************************************************************/
  ZilchDefineType(Engine, "Daisy", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Methods
    ZilchBindMethod(builder, type, &Engine::getGameSession, ZilchNoOverload, "getGameSession", ZilchNoNames);
    ZilchBindProperty(builder, type, &Mouse::Access, ZilchNoSetter, "Mouse", ZilchNoNames)->IsStatic;
    ZilchBindProperty(builder, type, &Keyboard::Access, ZilchNoSetter, "Keyboard", ZilchNoNames)->IsStatic;

    // Connect to entity
    Zilch::ParameterArray connectEntity;
    connectEntity.push_back(ZilchTypeId(Entity));
    connectEntity.push_back(ZilchTypeId(Zilch::String));
    connectEntity.push_back(Zilch::Core::GetInstance().AnyDelegateType);
    builder.AddBoundFunction(type, "Connect", DCEngine::ZilchConnect, connectEntity, ZilchTypeId(void), Zilch::FunctionOptions::Static);
  }

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
    // Construct all of the engine's systems
    ConstructSystems();
    // Create the default gamesession object, the "game" itself,  which contains all spaces.
    CurrentGameSession.reset(new GameSession(_projectName));
    // Load the default space to start with
    LoadDefaultSpace();
    // Initialize all internal engine systems
    for (auto sys : Systems) {
      sys->Initialize();
    }  
    // Load all engine's resources, single-threadedly
    getSystem<Systems::Content>()->LoadAllResources(false);
    // Subscribe to events
    Subscribe();
    DCTrace << "[Engine::Initialize - All engine systems initialized]\n";
    // Initialize the gamesession. (This will initialize its spaces,
    // and later, its gameobjects)
    CurrentGameSession->Initialize();      
    
    // Open the last known recent project on a separate thread. 
    // While it's loading, we will be displaying a loading screen.
    getSystem<Systems::Editor>()->OpenRecentProject();    

    Systems::DispatchSystemEvents::EngineInitialized();
    // Display a loading screen
    DisplayLoadingScreen(true);

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

  /**************************************************************************/
  /*!
  @brief  Received when the current project has been loaded.
  */
  /**************************************************************************/
  void Engine::OnContentProjectLoadedEvent(Events::ContentProjectLoaded * event)
  {
    DisplayLoadingScreen(false);
  }

  /**************************************************************************/
  /*!
  @brief  Received when the Window loses focus.
  */
  /**************************************************************************/
  void Engine::OnWindowLostFocusEvent(Events::WindowLostFocus * event)
  {
    Systems::DispatchSystemEvents::EnginePause();
    DispatchGameEvents::GameFocusOut();
    
  }

  /**************************************************************************/
  /*!
  @brief  Received when the Window regains focus.
  */
  /**************************************************************************/
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
    if (DCE_TRACE_PAUSE)
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
    if (DCE_TRACE_PAUSE)
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
    Configurations.Content.Save();
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
  @brief Connects an object to an event on an entity.
  @param eventName The name of the event.
  @param publisher A reference to the entity to connect to.
  @param deleg A pointer to the delegate.
  @param inst The instance of the object which will connect.
  @note This method is routed to by both C++ and Zilch components.
  */
  /**************************************************************************/
  void Engine::ConnectTo(const std::string & eventName, Entity* publisher, EventDelegate * deleg, Component * inst)
  {
    // Store the base delegate to the <EventClass, std::list<EventDelegate*> > map
    publisher->ObserverRegistryByString[eventName].emplace_back(deleg);
    // Add a pointer to the publishing entity in the component
    inst->ActiveDelegateHolders.push_back(publisher);
  }

  /**************************************************************************/
  /*!
  @brief Disconnects a ZilchComponent from a particular event on an entity.
  @param call A reference to the call object.
  @param report A reference to the report object.
  */
  /**************************************************************************/
  void Engine::ZilchDisconnect(Zilch::Call & call, Zilch::ExceptionReport & report)
  {
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