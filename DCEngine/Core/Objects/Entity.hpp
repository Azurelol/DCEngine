/******************************************************************************/
/*!
@file      Entity.hpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      8/18/2015
@brief     Template files.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Entity.h"

namespace DCEngine {

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
  inline bool Entity::AddComponent(bool initialize)
  {
    // If there is already a component of the same class, reject the operation
    for (auto &componentOwned : ComponentsContainer) {
      if (std::type_index(typeid(*componentOwned.get())) == (std::type_index(typeid(ComponentClass)))) {
        DCTrace << ObjectName << "::AddComponent - Failure! " << componentOwned->Name() << " is already present!\n";
        return false;
      }
    }
    // Construct the component
    ComponentsContainer.emplace_back(ComponentStrongPtr(new ComponentClass(*this)));

    // Initialize the component
    if (initialize)
      ComponentsContainer.back().get()->Initialize();
    return true;
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
    for (auto &componentPtr : ComponentsContainer) {
      auto component = componentPtr.get();
      // If the component was found
      if (std::type_index(typeid(*component)) == std::type_index(typeid(ComponentClass)))
        return (reinterpret_cast<ComponentClass*>(component));
    }
    // Iterate through the container of Zilch component handles
    for (auto &componentHandle : ComponentHandlesContainer) {
      auto component = reinterpret_cast<Component*>(componentHandle.Dereference());
      // If the component was found
      if (std::type_index(typeid(*component)) == std::type_index(typeid(ComponentClass)))
        return (reinterpret_cast<ComponentClass*>(component));
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
  \param eventObj The event object that is being passed.
  */
  /**************************************************************************/
  template <typename EventClass>
  void Entity::Dispatch(Event * eventObj) {
    if (TRACE_DISPATCH)
      DCTrace << Name() << "::Dispatch - Sending event\n";

    // 1. Dispatch to events identified by type_index
    auto eventTypeID = std::type_index(typeid(EventClass));
    // Look for a matching event among the keys
    for (auto& eventKey : ObserverRegistry) {
      if (eventTypeID == eventKey.first) {
        if (TRACE_DISPATCH)
          DCTrace << Name() << "::Dispatch - Found delegates with matching event type!\n";
        // For every delegate in the list for this specific event
        for (auto& deleg : eventKey.second) {
          // Call the delegate's member function
          if (TRACE_DISPATCH)
            DCTrace << Name() << "::Dispatch - Calling member function on " << "\n";
          if (!deleg->Call(eventObj)) {
            DCTrace << Name() << "::Dispatch - Unable to call member function " << "\n";
            //ObserverRegistry.erase(deleg);
          }
          //deleg.Call<eventTypeID>(eventObj);
        }
      }
      else {
        if (TRACE_DISPATCH)
          DCTrace << Name() << "::Dispatch - No delegate with event type matched!\n";
      }
    }

    // 2. Dispatch to events identified by name
    Dispatch(eventObj);
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

  /**************************************************************************/
  /*!
  @brief  Deregisters an observer from this entity.
  @param  observer A pointer to the object listening to this entity.
  */
  /**************************************************************************/
  template<typename Class>
  inline void Entity::DeregisterObserver(Class * observer)
  {
    // 1. Look for the observer in the registry by type
    for (auto& event : ObserverRegistry) {
      // For every delegate in the list of delegates
      for (auto it = event.second.begin(); it != event.second.end(); ++it) {
        // If the observer the delegate is pointing to matches..
        if ((*it)->GetObserver() == observer) {
          event.second.erase(it);
          break;
        }
      }
    }
    // 2. Look for the observer in the second registry by name
    for (auto& event : ObserverRegistryByString) {
      // For every delegate in the list of delegates
      for (auto it = event.second.begin(); it != event.second.end(); ++it) {
        // If the observer the delegate is pointing to matches..
        if ((*it)->GetObserver() == observer) {
          event.second.erase(it);
          break;
        }
      }
    }

  }

}