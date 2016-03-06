/******************************************************************************/
/*!
@file   Engine.hpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/11/2015
@brief  The file that holds all template functions used by the Engine
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
//#pragma once
#include "Engine.h"
#include <ZILCH\Zilch.hpp>
namespace DCEngine {

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
  template<typename EventClass>
  void Engine::ZilchConnect(Entity* publisher, Zilch::Function* fn, ZilchComponent* inst) {

    auto zilchDeg = new EventZilchFunctionDelegate<ZilchComponent>();
    zilchDeg->FuncPtr = fn;
    zilchDeg->Inst = inst;

    // Create a base delegate pointer to pass to the entity's container
    auto degPtr = dynamic_cast<EventDelegate*>(zilchDeg);
    // Store the base delegate to the <EventClass, std::list<EventDelegate*> > map
    publisher->ObserverRegistry[typeid(EventClass)].emplace_back(degPtr);
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
    for (auto systemPtr : Systems) {
      if (std::type_index(typeid(*systemPtr.get())) == std::type_index(typeid(SystemClass)))
        return std::static_pointer_cast<SystemClass>(systemPtr);
    }
    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");
  }


  /**************************************************************************/
  /*!
  \brief  Loads a configuration file.
  \param The event class.
  \param The event object that is being passed.
  */
  /**************************************************************************/
  template<typename Type>
  inline bool Engine::LoadConfiguration(Type & config, std::string fileName)
  {
    std::string configData;

    // Attempt to load the Graphics Config
    if (FileSystem::FileReadToString(fileName, configData)) {
      Serialization::Deserialize(config, configData);
      DCTrace << "Engine::LoadConfigurationFiles - Successfully loaded '" << fileName << "'! \n";
      return true;
    }
    else {
      Type defaultConfig;
      Serialization::Serialize(defaultConfig, configData);
      FileSystem::FileWriteString(fileName, configData);
      DCTrace << "Engine::LoadConfigurationFiles - Failed to load '" << fileName << "'! \n";
    }
    return false;
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


/*#define CreateZilchConnect(EventName)                                \
\
   void ZilchConnect##EventName(Entity* publisher, Zilch::Function* fn, ZilchComponent* fnOwner) \
   {                                                                                          \
     Daisy->ZilchConnect<::DCEngine::Events::##EventName>(publisher, fn, fnOwner); \
   } \*/

  //CreateZilchConnect(LogicUpdate);

  /*void ZilchConnectLogicUpdate(Entity* publisher, Zilch::Function* fn, ZilchComponent* fnOwner)
  {
    Daisy->Connect<::DCEngine::Events::LogicUpdate>(publisher, fn, fnOwner);
  }*/
}
