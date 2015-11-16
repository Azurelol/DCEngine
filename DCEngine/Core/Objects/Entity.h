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
#include "../Engine/Event.h"
#include "..\EventsInclude.h"
#include "..\Systems\Serialization\Serialization.h"

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

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Entity, Object);
    #endif

    Entity(std::string name);
    ~Entity();
    void Initialize(); 
    void Terminate();

    // Properties
    void setArchetype(std::string);
    std::string getArchetype() const; 

    // Components    
    void RemoveComponentByName(std::string& componentName);
    bool AddComponent(ComponentPtr component);
    template <typename ComponentClass> void AddComponentByName();
    template <typename ComponentClass> ComponentClass* getComponent();    
    template <typename ComponentClass> bool HasComponent();

    template <typename ComponentClass> void RemoveComponentByName();
    void RemoveComponent(ComponentPtr component);    
    ComponentVec* AllComponents();

    // Events
    // Dispatches an event on object
    template <typename EventClass>
    void Dispatch(Event* eventObj); 
    //!< Dispatches an event to the object itself and up the tree to each parent    
    template <typename EventClass>
    void DispatchUp(Event* eventObj); 
    //!< Dispatches an event to the object itself and down to each children recursively
    template <typename EventClass>
    void DispatchDown(Event* eventObj); 

    EntityType Type() { return type_; }

  protected:

    //ComponentVec ObserversList; //!< A list of the current listeners to this object.
    ComponentVec ComponentsContainer; //!< The list of components attached to the entity.  
    EntityType type_;

  private:

    std::map<std::type_index, std::list<DCEngine::Delegate*>> ObserverRegistry;
    std::map<unsigned int, std::list<DCEngine::Component*>> RemovalRegistry;
    std::string ArchetypeName;

    template <typename GenericEvent, typename GenericComponent>
    unsigned int RegisterListener(GenericComponent*, void (GenericComponent::*)(DCEngine::Event*));
    

  }; // class Entity
  
  /**************************************************************************/
  /*!
  @brief  Adds a given component to the Entity.
  @param  ComponentClass The component class.
  @note   The component gets initialized after being added, since this
          method is called at runtime.
  @todo   Have the Factory construct the component
  */
  /**************************************************************************/
  template<typename ComponentClass>
  inline void Entity::AddComponentByName()
  {
    // Construct the component
    auto component = ComponentPtr(new ComponentClass(*this));
    // Add the component to the entity
    AddComponent(component);
    // Initialize the component
    component->Initialize();
  }

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
    for (auto componentPtr : ComponentsContainer) {
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
  @brief  Finds a component belonging to the entity and removes it.
  @param  The component class.  
  */
  /**************************************************************************/
  template<typename ComponentClass>
  inline void Entity::RemoveComponentByName()
  {
    // Iterate through the container of component pointers...
    for (auto componentPtr : ComponentsContainer) {
      auto component = componentPtr.get();
      // If the component was found
      if (std::type_index(typeid(*component)) == std::type_index(typeid(ComponentClass))) {
        // Removes the component
        RemoveComponent(componentPtr);
      }        
    }
  }

  /**************************************************************************/
  /*!
  \brief  Finds a component belonging to the entity.
  \param  The component class.
  \return If a match was found, true. Otherwise, false.
  */
  /**************************************************************************/
  template<typename ComponentClass>
  inline bool Entity::HasComponent()
  {
    // Iterate through the container of component pointers...
    for (auto componentPtr : ComponentsContainer) {
      auto component = componentPtr.get();
      // If the component was found
      if (std::type_index(typeid(*component)) == std::type_index(typeid(ComponentClass)))
        return true;
    }
    // No matching component was found
    return false;
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
        DCTrace << Name() << "::Dispatch - Sending event\n";

      // For every delegate in the registry
      auto eventTypeID = std::type_index(typeid(EventClass));
      // Look for a matching event among the keys
      for (auto& eventKey : ObserverRegistry) {
        if (eventTypeID == eventKey.first) {
          if (TRACE_DISPATCH)
            DCTrace << Name() << "::Dispatch - Found delegates with matching event type!\n";
          // For every delegate in the list for this specific event
          for (auto deleg : eventKey.second) {
            // Call the delegate's member function
            if (TRACE_DISPATCH)
              DCTrace << Name() << "::Dispatch - Calling member function on " << "\n";
            deleg->Call(eventObj);
            //deleg.Call<eventTypeID>(eventObj);
          }
        }
        else {
          if (TRACE_DISPATCH)
            DCTrace << Name() << "::Dispatch - No delegate with event type matched!\n";
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
        DCTrace << Name() << "::DispatchDown - Sending event\n";

      // For every delegate in the registry
      auto eventTypeID = std::type_index(typeid(EventClass));
      // Look for a matching event among the keys
      for (auto& eventKey : ObserverRegistry) {
        if (TRACE_DISPATCH)
          DCTrace << Name() << "::Dispatch - Looking for the event" << " by typeid through the registry\n";
        if (eventTypeID == eventKey.first) {
          if (TRACE_DISPATCH)
            DCTrace << Name() << "::Dispatch - Found delegates with matching event type!\n";
          // For every delegate in the list for this specific event
          for (auto deleg : eventKey.second) {
            // Call the delegate's member function
            if (TRACE_DISPATCH)
              DCTrace << Name() << "::Dispatch - Calling member function on " << deleg.componentPtr->Name() << "\n";
            deleg.Call(eventObj);
          }
        }
        else {
          if (TRACE_DISPATCH)
            DCTrace << Name() << "::Dispatch - No delegate with event type matched!\n";
        }
      }
    }

  using EntityPtr = std::shared_ptr<Entity>;
  using EntityVec = std::vector<EntityPtr>;

} // DCEngine
#endif