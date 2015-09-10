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

#include <memory> // unique_ptr
#include <unordered_map>
#include <stack>
#include <functional> // std::function, std::bind

#include "..\Objects\Entities\GameSession.h"
#include "..\Objects\Entities\Space.h"
#include "..\Objects\Entity.h"
#include "..\Objects\Component.h"
#include "..\Systems\System.h"
#include "..\Systems\Gamestate\Gamestate.h"

#include "..\..\Gamestates\GamestatesInclude.h"
#include "..\Systems\SystemsInclude.h"
#include "..\Events\Event.h"
#include "..\Events\Delegate.h"

//extern std::string DefaultSpace = "Daisy World";

namespace DCEngine {
  
  class Engine {
    public:

      Engine();
      ~Engine();

      void Initialize();
      void Update(float dt);
      void Loop();
      auto Stop() { _active = false; } // Signals the engine to stop running
      void Terminate();
      
      auto Getdt() { return dt; }
            
      template <typename GenericEvent, typename GenericComponent, typename MemberFunction>
      void Connect(Entity* entity, MemberFunction fn, GenericComponent* comp);
      //void Disconnect(const Entity& entity, EventType);

      template<typename T> std::shared_ptr<T> GetSystem(EnumeratedSystem sysType);

     private:

      /*/ VARIABLES /*/
      GameSessionPtr _gameSession; //!< The current GameSession object.
      float dt; //!< Delta time. 
      float _framerate = 60.0f; //!< The target frame rate.
      float _runtime; //!< How long the engine has been running.
      bool _active; //!< Whether the engine is active or not.
      std::string _projectName = "Daisy Project"; //!< The current project.
      std::string _defaultSpace = "Daisy World"; 

      /*/ CONTAINERS /*/
      SystemVec _systems; //!< Container for the engine's systems.   
      SpaceMap _spaces; //!< A map of spaces created by the engine.      

      /* Reference: http://www.cplusplus.com/reference/stack/stack/
      Stacks are a type of container adaptor, specifically designed to operate in a 
      LIFO context (last-in first-out), where elements are inserted and extracted only 
      from one end of the container.
      */
            
      
  }; // Engine
  
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
    template<typename GenericEvent, typename GenericComponent, typename MemberFunction>
    void Engine::Connect(Entity* entity, MemberFunction fn, GenericComponent* comp) {

      if (TRACE_CONNECT) {
        trace << "Engine::Connect - " << comp->Name() << " has connected to " 
              << entity->Name() << " for event  " << "\n";   
      }

      // Construct the delegate
      Delegate dg;
      //dg.Create(comp, fn);

      // Store it in the subject's (the entity) listener registry. When the entity receives 
      // an event of that type, the methods on the listener will be called.
      

      // Testlol
      Events::UpdateEvent eventObj;
      fn(comp, &eventObj); // Test: Calls member function




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
  std::shared_ptr<T> Engine::GetSystem(EnumeratedSystem sysType) {
    for (auto &it : _systems) {
      if (it->_type == sysType)
        return std::static_pointer_cast<T>(it);
    }
    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");
  }

  /////////////////////////////////////////
  /*/ MACROS ( I AM SORRY VOLPER, MEAD )/*/
  /////////////////////////////////////////
  // Expansion macro for system
  #define GETSYSTEM( systype ) \
  Daisy->GetSystem<systype>(EnumeratedSystem::##systype)

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
  #define Connect(EntityObj, Event, Function) Daisy->Connect<::DCEngine::Event>( (Entity*)(EntityObj), std::mem_fn(&Function), this)
  
  //#define CONNECT(EVENT_TYPE, ENTITY, MEMFN) Daisy->Connect<::DCEngine::EVENT_TYPE>(ENTITY, MEMFN, this)

} // DCEngine