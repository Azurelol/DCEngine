/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The main driver for the Daisy Chain engine, and its engine class,
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Libraries
#include <unordered_map>
#include <stack>
#include <functional> // std::function, std::bind

// Headers
#include "Types.h"
#include "Event.h"
#include "Math.h"
#include "Action.h"

#include "../Objects/ObjectsInclude.h"
#include "../Objects/Entities/EntitiesInclude.h"
#include "..\Systems\System.h"
#include "..\Systems\SystemsInclude.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;

  class Engine : public Object {
    friend class EngineLauncher;
  public:

    void Register(Action& action);
    void Deregister(Action& action);

    Engine();
    Engine(std::string configFile);
    ~Engine();
    void Initialize();
    void Loop();
    void Terminate();
    auto Stop() { _active = false; }
    void LoadProject(std::string& filename);
    void StartProject();    

    Keyboard* getKeyboard() { return keyboard_.get(); }
    Mouse* getMouse() { return mouse_.get(); }
    Systems::Factory& getFactory() {
      return *getSystem<Systems::Factory>(EnumeratedSystem::Factory);
    }
    GameSession* getGameSession() { return CurrentGameSession.get(); }

    // Component Events
    template <typename EventClass, typename Class, typename MemberFunction>
    void Connect(Entity* publisher, MemberFunction fn, Class* inst);
    template <typename Publisher, typename Observer>
    void Disconnect(Publisher* publisher, Observer* observer);

    // System Events
    template<typename EventClass, typename SystemClass, typename MemberFunction>
    void Connect(MemberFunction fn, SystemClass* sys);
    template <typename EventClass> void Dispatch(Event* eventObj);

    template<typename T> std::shared_ptr<T> getSystem(EnumeratedSystem sysType);
    template<typename T> std::shared_ptr<T> getSystem();

  private:

    bool Paused;
    EngineConfigPtr EngineConfiguration;
    GameSessionPtr CurrentGameSession; //!< The current GameSession object.
    KeyboardPtr keyboard_;
    MousePtr mouse_;
    float dt; //!< Delta time. 
    float _framerate = 60.0f; //!< The target frame rate.
    float _runtime; //!< How long the engine has been running.
    bool _active; //!< Whether the engine is active or not.      
    std::string _projectName = "Daisy Project"; //!< The current project.
    std::string _defaultSpace = "Daisy World";
    SystemVec _systems; //!< Container for the engine's systems.   
    SpaceMap _spaces; //!< A map of spaces created by the engine.
    ActionSpace ActionSpace; 
    std::map<std::type_index, std::list<DCEngine::EventDelegate*>> ObserverRegistry;

    bool LoadEngineConfig();
    void LoadDefaultSpace();
    // Updates
    void Update(float dt);   
    void DispatchUpdateEvents(float dt);    
    void UpdateActions(float dt);
    // Events
    void Subscribe();
    void OnWindowLostFocusEvent(Events::WindowLostFocus* event);
    void OnWindowGainedFocusEvent(Events::WindowGainedFocus* event);    
    void OnEnginePauseEvent(Events::EnginePause* event);
    void OnEngineResumeEvent(Events::EngineResume* event);
    void OnEngineExitEvent(Events::EngineExit* event);
    void OnEnginePauseMenuEvent(Events::EnginePauseMenu* event);


  }; // Engine. Template definitions are found below.

  /**************************************************************************/
  /*!
  \brief  Subscribes a component to an entity, registering it to its listeners'
  registry for the specific event.
  \note   A very helpful 'Connect' macro has been provided to simplify the call.
  \param  A pointer to the entity.
  \param  A member function from the component.
  \param  A pointer to the component.
  \return A shared pointer to the requested system.
  */
  /**************************************************************************/
  template<typename EventClass, typename Class, typename MemberFunction>
  void Engine::Connect(Entity* publisher, MemberFunction fn, Class* inst) {

    //if (TRACE_CONNECT) {
    //  DCTrace << "[Engine::Connect] - " << comp->Name() << " has connected to "
    //    << entity->Name() << "\n";
    //}
    
    // Construct the member function delegate
    auto memDeg = new EventMemberFunctionDelegate<Class, EventClass>();
    memDeg->FuncPtr = fn;
    memDeg->Inst = inst;
    // Create a base delegate pointer to pass to the entity's container
    auto degPtr = dynamic_cast<EventDelegate*>(memDeg);
    // Store the base delegate to the <EventClass, std::list<EventDelegate*> > map
    publisher->ObserverRegistry[typeid(EventClass)].emplace_back(degPtr);
    //publisher->ObserverRegistry[typeid(EventClass)].push_back(degPtr);
    // Add a pointer to entiyy
    inst->ActiveDelegateHolders.push_back(publisher);
  }


  template<typename Publisher, typename Observer>
  inline void Engine::Disconnect(Publisher * publisher, Observer * observer)
  {
    publisher->DeregisterObserver(observer);
  }

  /**************************************************************************/
  /*!
  \brief  Subscribes a system's member function to an engine event.
  \param  A pointer to the engine instance.
  \param  A member function from the component.
  \param  A pointer to the component.
  \return A shared pointer to the requested system.
  */
  /**************************************************************************/
  template<typename EventClass, typename SystemClass, typename MemberFunction>
  void Engine::Connect(MemberFunction fn, SystemClass* sys) {

    // Construct the member function delegate
    auto memDeg = new EventMemberFunctionDelegate<SystemClass, EventClass>();
    memDeg->FuncPtr = fn;
    memDeg->Inst = sys;
    // Create a base delegate pointer to pass to the entity's container
    auto degPtr = (EventDelegate*)memDeg;
    // Store the base delegate to the <EventClass, std::list<EventDelegate*> > map
    this->ObserverRegistry[typeid(EventClass)].push_back(degPtr);
  }

  /**************************************************************************/
  /*!
  \brief  Dispatches an event to the engine.
  \param The event class.
  \param The event object that is being passed.
  */
  /**************************************************************************/
  template <typename EventClass>
  void Engine::Dispatch(Event * eventObj) {

    // For every delegate in the registry
    auto eventTypeID = std::type_index(typeid(EventClass));
    // Look for a matching event among the keys
    for (auto& eventKey : ObserverRegistry) {
      if (eventTypeID == eventKey.first) {
        // For every delegate in the list for this specific event
        for (auto deleg : eventKey.second) {
          // Call the delegate's member function
          if (!deleg->Call(eventObj)) {
            DCTrace << "Engine::Dispatch - Event failed!\n";
            // If the call failed because the object instance is
            // no longer available, remove this delegate
            // Erase by iterator!
            //ObserverRegistry.find(deleg);
            //ObserverRegistry.erase(it);
          }
          //deleg.Call<eventTypeID>(eventObj);
        }
      }
    }
  }
  
  /**************************************************************************/
  /*!
  \brief  Allows access to a system running in the engine via system type.
  \para   SystemClass The class of the system.
  \return A shared pointer to the requested system.
  */
  /**************************************************************************/
  template<typename SystemClass>
  inline std::shared_ptr<SystemClass> Engine::getSystem() {
    for (auto systemPtr : _systems) {
      if (std::type_index(typeid(*systemPtr.get())) == std::type_index(typeid(SystemClass)))
        return std::static_pointer_cast<SystemClass>(systemPtr);
    }
    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");
  }

  /////////////////////////////////////////
  /*/ MACROS ( I AM SORRY VOLPER, MEAD )/*/
  /////////////////////////////////////////
  // Expansion macro for system
#define GETSYSTEM( systype ) \
  Daisy->getSystem<systype>(EnumeratedSystem::##systype)

  /**************************************************************************/
  /*!
  @brief  A macro to simplify the Connect function. It subscribes to an entity's
          events, receiving them when they are called on the function provided.
  @param  EntityObj A pointer to an Entity-derived object which gets casted to
          a base Entity pointer
  @param  Event A specifier for a specific event (via enum)
  @param  Function A member function belonging to the component, gets passed
          into the function by the std::mem_fn wrapper for member functions.
  */
  /**************************************************************************/
  //#define Connect(EntityObj, Event, Function) Daisy->Connect<::DCEngine::Event>( (Entity*)(EntityObj), std::mem_fn(&Function), this)
  #define Connect(EntityObj, Event, Function) Daisy->Connect<::DCEngine::Event>( (Entity*)(EntityObj), &Function, this)

  /*
  The interface for launching Daisy Chain engine.
  */
  class EngineLauncher {
  public:
    EngineLauncher() {
      engine_.reset(new Engine());
    }
    void InitializeEngine() {
      engine_->Initialize();
    }
    void StartLoop() {
      engine_->Loop();
    }
    void TerminateEngine() {
      engine_->Terminate();
    }

  private:
    std::unique_ptr<Engine> engine_;
  };

} // DCEngine