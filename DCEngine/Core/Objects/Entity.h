/******************************************************************************/
/*!
\file   Entity.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The base object composition class.

*/
/******************************************************************************/
#ifndef ENTITY_H
#define ENTITY_H
#pragma once

// Base class
#include "Object.h"
// Libraries
#include <map>
#include <list>
// Headers
#include "Component.h"
#include "..\Events\Event.h"
#include "..\Events\EventsInclude.h"

namespace DCEngine {
  
  enum class EntityType {
    GameObject,
    Space,
    GameSession,
  };

  class Engine; // The engine has complete access to entities.
  class Space;
  class GameSession;

  class Entity : public Object {
    friend class Engine;
    friend class Space;

  public:
    Entity(std::string name);
    Entity() { _name = "Entity"; }

    void Initialize(); //!< Initializes all of the entity's components
    void Update(float dt); //!< Updates all the entity's components directly. (TEMPORARY)
    EntityType Type() { return type_; }

    void AddComponent(ComponentPtr component);
    Component* GetComponent(std::string& name);

    void RemoveComponent(EnumeratedComponent ec);
    bool HasComponent(EnumeratedComponent ec);    

    // VIRTUAL FUNCTIONS //
    //virtual void SetParentReference();

    // EVENTS // 

    template <typename EventClass>
    void Dispatch(Event* eventObj); // Dispatches an event on object
    template <typename EventClass>
    void DispatchUp(Event* eventObj); //!< Dispatches an event to the object itself and up the tree to each parent    
    template <typename EventClass>
    void DispatchDown(Event* eventObj); //!< Dispatches an event to the object itself and down to each children recursively
    
    bool CheckMask(mask m);
    // Allows access to attached components of the entity.
    #define GET_COMPONENT(type) \ GetComponent<type>(EnumeratedSystem::##type);
    template <typename T> std::shared_ptr<T> GetComponent(EnumeratedComponent ec);

  protected:
    ComponentVec _observers; //!< A list of the current listeners to this object.
    ComponentVec _components; //!< The list of components attached to the entity.  
    EntityType type_;

  private:
    ////////////////////////////////
    // PRIVATE MEMBER [FUNCTIONS] 
    template <typename GenericEvent, typename GenericComponent>
    unsigned int RegisterListener(GenericComponent*, void (GenericComponent::*)(DCEngine::Event*));

    //ComponentPtr _components[static_cast<int>(EnumeratedComponent::Capacity)];

    /////////////////////////////////
    // PRIVATE MEMBER [VARIABLES]
    Entity* _parent; //!< The entity to which this object is parented to.
    std::string _archetypeName;    
    mask _mask = static_cast<int>(BitfieldComponent::Alive);
    
    
    // Reference: http://www.cplusplus.com/reference/map/map/
    // http://stackoverflow.com/questions/9859390/use-data-type-class-type-as-key-in-a-map
    std::map<std::type_index, std::list<DCEngine::Delegate>> ObserverRegistry;
    std::map<unsigned int, std::list<DCEngine::Delegate>> ListenerRegistry;
    std::map<unsigned int, std::list<DCEngine::Component*>> RemovalRegistry;
    

    //EnumeratedComponent _collider = EnumeratedComponent::None;

  }; // class Entity

     /**************************************************************************/
     /*!
     \brief  Dispatches an event to the object.
     \param The event class
     \param The event object that is being passed.
     */
     /**************************************************************************/
    template <typename EventClass>
    void Entity::Dispatch(Event * eventObj) {
      if (TRACE_DISPATCH)
        trace << Name() << "::Dispatch - Sending event\n";

      // For every delegate in the registry
      auto eventTypeID = std::type_index(typeid(EventClass));
      // Look for a matching event among the keys
      for (auto& eventKey : ObserverRegistry) {
        if (eventTypeID == eventKey.first) {
          if (TRACE_DISPATCH)
            trace << Name() << "::Dispatch - Found delegates with matching event type!\n";
          // For every delegate in the list for this specific event
          for (auto& deleg : eventKey.second) {
            // Call the delegate's member function
            if (TRACE_DISPATCH)
              trace << Name() << "::Dispatch - Calling member function on " << deleg.componentPtr->Name() << "\n";
            deleg.Call(eventObj);
            //deleg.Call<eventTypeID>(eventObj);
          }
        }
        else {
          if (TRACE_DISPATCH)
            trace << Name() << "::Dispatch - No delegate with event type matched!\n";
        }
      }



    }

    /**************************************************************************/
    /*!
    \brief  Dispatches an event to the object itself and down to each children 
            recursively
    \return True if the component has every specified component.
    */
    /**************************************************************************/
    template <typename EventClass>
    void Entity::DispatchDown(Event * eventObj) {
      if (TRACE_DISPATCH)
        trace << Name() << "::DispatchDown - Sending event\n";

      // For every delegate in the registry
      auto eventTypeID = std::type_index(typeid(EventClass));
      // Look for a matching event among the keys
      for (auto& eventKey : ObserverRegistry) {
        if (TRACE_DISPATCH)
          trace << Name() << "::Dispatch - Looking for the event" << " by typeid through the registry\n";
        if (eventTypeID == eventKey.first) {
          if (TRACE_DISPATCH)
            trace << Name() << "::Dispatch - Found delegates with matching event type!\n";
          // For every delegate in the list for this specific event
          for (auto& deleg : eventKey.second) {
            // Call the delegate's member function
            if (TRACE_DISPATCH)
              trace << Name() << "::Dispatch - Calling member function on " << deleg.componentPtr->Name() << "\n";
            deleg.Call(eventObj);
          }
        }
        else {
          if (TRACE_DISPATCH)
            trace << Name() << "::Dispatch - No delegate with event type matched!\n";
        }
      }
    }

  using EntityPtr = std::shared_ptr<Entity>;
  using EntityVec = std::vector<EntityPtr>;

} // DCEngine
#endif