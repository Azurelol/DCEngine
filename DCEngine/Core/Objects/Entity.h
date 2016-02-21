/******************************************************************************/
/*!
@file   Entity.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/18/2015
@brief  The base object composition class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

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
//#include "..\ComponentsInclude.h" // Entities need to know of componnets
#include "../Engine/Event.h"
#include "..\EventsInclude.h"
#include "..\Systems\Serialization\Serialization.h"
#include "../Engine/Types.h"
#include "../Engine/Action.h"

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

    ZilchDeclareDerivedType(Entity, Object);
    Entity(std::string name);
    ~Entity();
    void Initialize();
    void Terminate();
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);

    // Properties
    void setArchetype(std::string);
    std::string getArchetype() const;
    // Components    
    template<typename ComponentClass> bool AddComponent(bool initialize = false);
    ComponentPtr AddComponentByName(const std::string& name, bool initialize = false);
    bool AddComponentByType(Zilch::BoundType* boundType, bool initialize = false);
    //template <typename ComponentClass> bool AddComponent(bool initialize = false);
    template <typename ComponentClass> ComponentClass* getComponent();
    template <typename ComponentClass> bool HasComponent();
    bool HasComponent(const std::string& name);
    template <typename ComponentClass> void RemoveComponentByName();
    void RemoveComponentByName(std::string componentName);
    void RemoveComponent(ComponentPtr component);
    ComponentVec AllComponents();
    void Swap(ComponentPtr, Direction);
    void SwapToBack(ComponentPtr);

    // Events
    template <typename EventClass> void Dispatch(Event* eventObj);
    template <typename EventClass> void DispatchUp(Event* eventObj);
    template <typename EventClass> void DispatchDown(Event* eventObj);
    EntityType Type() { return type_; }
    // Actions
    ActionsOwner Actions;

  protected:

    ComponentStrongVec ComponentsContainer; //!< The list of components attached to the entity.  
    ComponentHandleVec ComponentHandlesContainer; //!< The list of components created through Zilch attached to the entity.
    EntityType type_;

  private:

    //! Use a vector here instead perhaps?
    std::map<std::type_index, std::list<std::unique_ptr<EventDelegate>>> ObserverRegistry;
    std::map<unsigned int, std::list<DCEngine::Component*>> RemovalRegistry;
    std::string ArchetypeName;
    bool IsInitialized = false;

    // Methods
    ComponentHandle GetComponentHandle(ComponentPtr component);
    template <typename GenericEvent, typename GenericComponent>
    unsigned int RegisterListener(GenericComponent*, void (GenericComponent::*)(DCEngine::Event*));
    template <typename Class> void DeregisterObserver(Class* observer);
    void InformObserversOfDeath();

  };

  using EntityPtr = Entity*;
  using EntityVec = std::vector<EntityPtr>;
}

#include "Entity.hpp"
#endif