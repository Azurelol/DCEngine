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
// Headers
#include "Component.h"
//#include "..\ComponentsInclude.h" // Entities need to know of componnets
#include "Event.h"
#include "..\EventsInclude.h"
#include "..\Systems\Serialization\Serialization.h"
#include "../Engine/Types.h"
#include "../Engine/Action.h"
// Macros
#define DCE_ENTITY_GET_COMPONENT(ComponentName)                    \
  Components::ComponentName* get##ComponentName() {                            \
    return this->getComponent<Components::ComponentName>();                    \
  }
#define DCE_BINDING_ENTITY_COMPONENT_AS_PROPERTY(ComponentName)    \
ZilchBindProperty(builder, type, &Entity::get##ComponentName, ZilchNoSetter, "" #ComponentName)

namespace DCEngine {

  enum class EntityType {
    GameObject,
    Space,
    GameSession,
  };

  class Engine; // The engine has complete access to entities.
  class Space;
  class GameSession;
  
  // Pointers
  class Entity;
  using EntityPtr = Entity*;
  using EntityVec = std::vector<EntityPtr>;

  /**************************************************************************/
  /*!
  @class EntityRange A range-based container of entity pointers.
  */
  /**************************************************************************/
  class EntityRange {
    using Iterator = EntityVec::iterator;
    Iterator Begin;
    Iterator End;

    bool isRange() { return Begin != End; }
    typename const Iterator beginRange() const { return Begin; }
    typename const Iterator endRange() const { return End; }
    typename Iterator beginRange() { return Begin; }
    typename Iterator endRange() { return End; }
    size_t sizeRange() const { return End - Begin; }
  
  public:
    EntityRange(const Iterator& begin, const Iterator& end) : Begin(begin), End(end) {}
    ZilchDeclareBaseType(EntityRange, Zilch::TypeCopyMode::ValueType);

    static void All(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void MoveNext(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void Current(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void IsNotEmpty(Zilch::Call& call, Zilch::ExceptionReport& report);
    static void Count(Zilch::Call& call, Zilch::ExceptionReport& report);

  };  

  /**************************************************************************/
  /*!
  @class Entity The main composition class for the engine.
  */
  /**************************************************************************/
  class Entity : public Object {
    friend class Engine;
    friend class Space;

  public:

    static EntityPtr IsA(ObjectPtr object);
    ZilchDeclareDerivedType(Entity, Object);
    Entity(std::string name);
    ~Entity();
    void Initialize();
    void Terminate();
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);
    void Rebuild();
    // Properties    
    void setArchetype(std::string);
    std::string getArchetype() const;
    DCE_DEFINE_PROPERTY(bool, ModifiedFromArchetype);
    // Components    
    template<typename ComponentClass> bool AddComponent(bool initialize = false);
    ComponentPtr AddComponentByName(const std::string& name, bool initialize = false);
    bool AddComponentByType(Zilch::BoundType* boundType, bool initialize = false);    
    template <typename ComponentClass> ComponentClass* getComponent();
    ComponentPtr getComponent(const std::string& name);
    ComponentPtr getComponent(Zilch::BoundType* type);
    template <typename ComponentClass> bool HasComponent();
    bool HasComponent(const std::string& name);    
    template <typename ComponentClass> void RemoveComponentByName();
    void RemoveAllComponents();
    void RemoveComponentByName(std::string componentName);
    void RemoveComponent(ComponentPtr component);    
    ComponentVec AllComponents();
    ComponentHandleVec& AllComponentsByHandle();    
    void Swap(ComponentPtr, Direction);
    void SwapToBack(ComponentPtr);
    // Events
    template <typename EventClass> void Dispatch(Event* eventObj);
    void Dispatch(std::string eventID, Event* event);
    void Dispatch(Event* eventObj);
    template <typename EventClass> void DispatchUp(Event* eventObj);
    template <typename EventClass> void DispatchDown(Event* eventObj);
    void RegisterListener(Zilch::Call & call, Zilch::ExceptionReport & report);
    EventDelegatesInfo PeekEvents();
    EntityType Type() { return type_; }
    // Actions
    ActionsOwner Actions;
    ActionsOwner& getActions() { return this->Actions; }
    // Parenting
    virtual EntityPtr Parent();
    virtual EntityPtr FindChildByName(std::string name);
    virtual EntityVec FindAllChildrenByName(std::string name);
    virtual EntityVec& Children();
    virtual EntityRange ChildrenByRange();
    virtual void AttachTo(EntityPtr parent) {}
    virtual void AttachToRelative(EntityPtr parent) {}
    virtual void Detach() {}
    virtual void DetachRelative() {}

  protected:
    EntityPtr ParentRef;
    EntityVec ChildrenContainer;
    unsigned ParentID;
    ComponentStrongVec ComponentsContainer; //!< The list of components attached to the entity.  
    ComponentHandleVec ComponentHandlesContainer; //!< The list of components created through Zilch attached to the entity.
    EntityType type_;

  private:

    std::string ArchetypeName;
    bool IsInitialized;
    bool ModifiedFromArchetype;
    // Events
    ObserverRegistryMapStr ObserverRegistryByString;
    ObserverRegistryMapTypeIndex ObserverRegistry;
    std::map<unsigned int, std::list<DCEngine::Component*>> RemovalRegistry;

    template <typename Class> void DeregisterObserver(Class* observer);    
    void InformObserversOfDeath();
    // Components
    ComponentHandle GetComponentHandle(ComponentPtr component);

  };
}

#include "Entity.hpp"
#endif