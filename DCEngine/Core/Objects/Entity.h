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
// Headers
#include "..\Events\Event.h"
#include "..\Events\EventsInclude.h"

#include "Component.h"

namespace DCEngine {
  
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

    void AddComponent(ComponentPtr component);
    void RemoveComponent(EnumeratedComponent ec);
    bool HasComponent(EnumeratedComponent ec);    

    // VIRTUAL FUNCTIONS //
    //virtual void SetParentReference();

    // EVENTS // 

    void Dispatch(Event& eventObj); // Dispatches an event on object
    void DispatchUp(); // Dispatches an event to the object itself and up the tree to each parent
    void DispatchDown(); // Dispatches an event to the object itself and down to each children recursively
    
    bool CheckMask(mask m);

    //EnumeratedComponent GetCollider();

    // Allows access to attached components of the entity.
    #define GET_COMPONENT(type) \ GetComponent<type>(EnumeratedSystem::##type);
    template <typename T> std::shared_ptr<T> GetComponent(EnumeratedComponent ec);

  protected:
    ComponentVec _observers; //!< A list of the current listeners to this object.
    ComponentVec _components; //!< The list of components attached to the entity.  


  private:    
	  Entity* _parent; //!< The entity to which this object is parented to.
    std::vector<EventType> _events; //!< The events the entity is currently subscribed to.
    //ComponentPtr _components[static_cast<int>(EnumeratedComponent::Capacity)];

  
    std::string _archetypeName;    
    mask _mask = static_cast<int>(BitfieldComponent::Alive);
    //EnumeratedComponent _collider = EnumeratedComponent::None;

  };

  using EntityPtr = std::shared_ptr<Entity>;
  using EntityVec = std::vector<EntityPtr>;

} // DCEngine
#endif