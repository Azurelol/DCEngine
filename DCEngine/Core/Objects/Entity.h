/******************************************************************************/
/*!
\file   Entity.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The main object composition class.

*/
/******************************************************************************/
#ifndef ENTITY_H
#define ENTITY_H
#pragma once

// Base class
#include "Object.h"
// Headers
#include "..\Events\Event.h"
#include "..\Events\EventsInclude.h"

#include "Component.h"
//class Component;

namespace DCEngine {

  class Entity : public Object {
  public:
    Entity(std::string name);
    Entity() { _name = "Entity"; }

    void Update(float dt);
    void AddComponent(ComponentPtr component);
    //void AddComponent(std::shared_ptr<Component> component);
    void RemoveComponent(EnumeratedComponent ec);
    bool HasComponent(EnumeratedComponent ec);
    void Initialize(); // Initializes all of the entity's components

    // EVENTS //
	  void Connect(); // The component has to register to an event to be updated.
	  void Disconnect(); // The component can unsubscribe from listening to an event.
    
    
    void Dispatch(std::string eventId, Event event); // Dispatches an event on object
    void DispatchUp(); // Dispatches an event to the object itself and up the tree to each parent
    void DispatchDown(); // Dispatches an event to the object itself and down to each children recursively

    mask Mask();
    bool CheckMask(mask m);

    //EnumeratedComponent GetCollider();

    // Allows access to attached components of the entity.
    #define GET_COMPONENT(type) \ GetComponent<type>(EnumeratedSystem::##type);
    template <typename T> std::shared_ptr<T> GetComponent(EnumeratedComponent ec);

  private:
    int RuntimeId;
    //Space Space;
    // EntityPtr _parent;
	  Entity* _parent;

    //ComponentPtr _components[static_cast<int>(EnumeratedComponent::Capacity)];

	ComponentVec _observers; //!< A list of the current listeners to this object.
    ComponentVec _components; //!< The list of components attached to the entity.
    std::string _archetypeName;    
    mask _mask = static_cast<int>(BitfieldComponent::Alive);
    //EnumeratedComponent _collider = EnumeratedComponent::None;

  };

  using EntityPtr = std::shared_ptr<Entity>;
  using EntityVec = std::vector<EntityPtr>;

} // DCEngine
#endif