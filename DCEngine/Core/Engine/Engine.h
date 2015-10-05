/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The main driver for the Daisy Chain engine, and its engine class,

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

#include "../Objects/ObjectsInclude.h"
#include "../Objects/Entities/EntitiesInclude.h"
#include "..\Systems\System.h"
#include "..\Systems\SystemsInclude.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;

  // Temporary, perhaps an entity that receives events for the engine.  
  class Engine {
    friend class EngineLauncher;
  public:

    Engine();
    ~Engine();

    void Initialize();
    void Loop();
    void Terminate();
    auto Stop() { _active = false; } // Signals the engine to stop running

    Keyboard* getKeyboard() { return keyboard_.get(); }
    Mouse* getMouse() { return mouse_.get(); }
    Systems::Factory& getFactory() {
      return *getSystem<Systems::Factory>(EnumeratedSystem::Factory);
    }
    template <typename EventClass, typename ComponentClass, typename MemberFunction>
    void Connect(Entity* entity, MemberFunction fn, ComponentClass* comp);
    //void Disconnect(const Entity& entity, EventType);

    template<typename T> std::shared_ptr<T> getSystem(EnumeratedSystem sysType);
    template<typename T> std::shared_ptr<T> getSystem();

  private:

    GameSessionPtr gamesession_; //!< The current GameSession object.
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

    void Update(float dt);
    void DispatchUpdateEvents(float dt);
    void LoadProject(std::string& filename); //!< Load a project from a filename


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
  template<typename EventClass, typename ComponentClass, typename MemberFunction>
  void Engine::Connect(Entity* entity, MemberFunction fn, ComponentClass* comp) {

    if (TRACE_CONNECT) {
      trace << "[Engine::Connect] - " << comp->Name() << " has connected to "
        << entity->Name() << "\n";
    }

    // Construct the member function delegate
    auto memDeg = new MemberFunctionDelegate<ComponentClass, EventClass>();
    memDeg->FuncPtr = fn;
    memDeg->CompInst = comp;
    // Create a base delegate pointer to pass to the entity's container
    auto degPtr = (Delegate*)memDeg;
    // Store the base delegate to the <EventClass, std::list<Delegate*> > map
    entity->ObserverRegistry[typeid(EventClass)].push_back(degPtr);
  }

  /**************************************************************************/
  /*!
  \brief  Allows access to a system running in the engine via system type.
  \note   This function call can be made much easier using the GETSYSTEM
  macro that takes a system typename and expands it to fill the
  template parameters.
  \para   An enumerated system.
  \return A shared pointer to the requested system.
  */
  /**************************************************************************/
  template <typename T>
  std::shared_ptr<T> Engine::getSystem(EnumeratedSystem sysType) {
    for (auto &it : _systems) {
      if (it->_type == sysType)
        return std::static_pointer_cast<T>(it);
    }
    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");
  }

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
  \brief  A macro to simplify the Connect function. It subscribes to an entity's
  events, receiving them when they are called on the function provided.
  \param  EntityObj A pointer to an Entity-derived object which gets casted to
  a base Entity pointer
  \param  Event A specifier for a specific event (via enum)
  \param  Function A member function belonging to the component, gets passed
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