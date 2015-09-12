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
    ////////////////////////////////
    // PRIVATE MEMBER [FUNCTIONS] 
    template <typename GenericEvent, typename GenericComponent>
    unsigned int RegisterListener(GenericComponent*, void (GenericComponent::*)(DCEngine::Event*));

    //ComponentPtr _components[static_cast<int>(EnumeratedComponent::Capacity)];

    /////////////////////////////////
    // PRIVATE MEMBER [VARIABLES]
    Entity* _parent; //!< The entity to which this object is parented to.
    std::vector<EventType> _events; //!< The events the entity is currently subscribed to.
    std::string _archetypeName;    
    mask _mask = static_cast<int>(BitfieldComponent::Alive);
    
    // Reference: http://www.cplusplus.com/reference/map/map/
    // http://stackoverflow.com/questions/9859390/use-data-type-class-type-as-key-in-a-map
    std::map<std::type_index, std::list<DCEngine::Delegate>> ObserverRegistry;
    std::map<unsigned int, std::list<DCEngine::Delegate>> ListenerRegistry;
    std::map<unsigned int, std::list<DCEngine::Component*>> RemovalRegistry;
    

    //EnumeratedComponent _collider = EnumeratedComponent::None;

  };

  using EntityPtr = std::shared_ptr<Entity>;
  using EntityVec = std::vector<EntityPtr>;

} // DCEngine
#endif