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
#include "..\Systems\Factory\Serializer.h"
#include "Component.h"
#include "../Engine/Event.h"
#include "..\EventsInclude.h"

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
    Entity() { ObjName = "Entity"; }
    void Initialize(); //!< Initializes all of the entity's components
    virtual void Serialize(Json::Value& root) = 0;
    virtual void Deserialize(Json::Value& root) = 0;
                    
    void AddComponentByName(std::string& componentName);
    void RemoveComponentByName(std::string& componentName);
    bool AddComponent(ComponentPtr component);
    template <typename ComponentClass> ComponentClass* getComponent();    
    Component* getComponentByName(std::string name);
    void RemoveComponent(EnumeratedComponent ec);
    bool HasComponent(EnumeratedComponent ec);

    template <typename EventClass>
    void Dispatch(Event* eventObj); // Dispatches an event on object
    template <typename EventClass>
    void DispatchUp(Event* eventObj); //!< Dispatches an event to the object itself and up the tree to each parent    
    template <typename EventClass>
    void DispatchDown(Event* eventObj); //!< Dispatches an event to the object itself and down to each children recursively
    
    bool CheckMask(mask m);
    EntityType Type() { return type_; }

  protected:

    ComponentVec observers_; //!< A list of the current listeners to this object.
    ComponentVec components_; //!< The list of components attached to the entity.  
    EntityType type_;

  private:

    std::map<std::type_index, std::list<DCEngine::Delegate*>> ObserverRegistry;
    std::map<unsigned int, std::list<DCEngine::Component*>> RemovalRegistry;
    Entity* ParentRef;
    std::string Archetype;
    mask _mask = static_cast<int>(BitfieldComponent::Alive);

    template <typename GenericEvent, typename GenericComponent>
    unsigned int RegisterListener(GenericComponent*, void (GenericComponent::*)(DCEngine::Event*));
    

  }; // class Entity
  
  /**************************************************************************/
  /*!
  \brief  Finds a component belonging to the entity.
  \param  The component class.
  \return If a match was found, a pointer to the component. If not, NULL.
  */
  /**************************************************************************/
  template<typename ComponentClass>
  ComponentClass* Entity::getComponent() { 
    // Iterate through the container of component pointers...
    for (auto componentPtr : components_) {
      auto component = componentPtr.get();
      // If the component was found
      if (std::type_index(typeid(*component)) == std::type_index(typeid(ComponentClass)))
        return (dynamic_cast<ComponentClass*>(component));
    }
    // No matching component was found
    return NULL;
  }

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
          for (auto deleg : eventKey.second) {
            // Call the delegate's member function
            if (TRACE_DISPATCH)
              trace << Name() << "::Dispatch - Calling member function on " << "\n";
            deleg->Call(eventObj);
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
          for (auto deleg : eventKey.second) {
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