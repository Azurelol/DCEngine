/******************************************************************************/
/*!
@file   Entity.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Entity.h"

// Headers
//#include "..\ComponentsInclude.h" // Entities need to know of componnets
#include "Entities\Space.h"
#include "Entities\GameSession.h"

#include "../Engine/Engine.h" // noooo!

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Entity constructor.
  */
  /**************************************************************************/
  Entity::Entity(std::string name) : Object("Entity"), ArchetypeName("") {
    ObjectName = name;
  }

  /**************************************************************************/
  /*!
  \brief  Entity destructor.
  */
  /**************************************************************************/
  Entity::~Entity()
  {
    if (DCE_TRACE_GAMEOBJECT_DESTRUCTOR)
      DCTrace << Name() << "::~Entity - Destructor called! \n";
    // 1. Remove all components from the entity
    ComponentsContainer.clear();
    /*for (auto component : ComponentsContainer)
      RemoveComponent(component);*/
  }


  /**************************************************************************/
  /*!
  \brief  Adds a component to the entity by pointer.
  \param  A component pointer to the component being added to the entity.
  */
  /**************************************************************************/
  //bool Entity::AddComponent(std::shared_ptr<Component> component) {
  //  //// If there is already a component of the same class, reject the operation
  //  //for (auto componentOwned : ComponentsContainer) {
  //  //  if (std::type_index(typeid(*componentOwned.get())) == (std::type_index(typeid(*component.get())))) {
  //  //    DCTrace << ObjectName << "::AddComponent - Failure! " << component->Name() << " is already present!\n";
  //  //    return false;
  //  //  }        
  //  //}

  //  //if (TRACE_COMPONENT_ADD)
  //  //  DCTrace << ObjectName << "::AddComponent - Added " << component->Name() << "\n";
  //  //// Adds the component to the entity
  //  //ComponentsContainer.push_back(component);
  //  //return true;    
  //}

  /**************************************************************************/
  /*!
  \brief  Initialize all of the entity's components.
  */
  /**************************************************************************/
  void Entity::Initialize() {
    // If the entity has already been initialized, do naught
    if (IsInitialized) {
      DCTrace << ObjectName << "::Initialize - Failed! Already initialized!\n";
      return;
    }      

    // Auto will never deduce to a reference because by default. Because
    // unique_ptrs do not allow copying, this wouldn't work otherwise.
    for (auto &component : ComponentsContainer)
      component.get()->Initialize();
    // Flag this entity as already being initialized
    IsInitialized = true;
    DCTrace << ObjectName << "::Initialize \n";
  }

  /**************************************************************************/
  /*!
  \brief Terminates the Entity.
  */
  /**************************************************************************/
  void Entity::Terminate()
  {

  }

  /**************************************************************************/
  /*!
  @brief Serializes an entity
  @param builder A reference to the JSON builder.
  @note  This will serialize the entity's properties, then its components.
  */
  /**************************************************************************/
  void Entity::Serialize(Zilch::JsonBuilder & builder)
  {
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    // Serialize this entity's name    
    
    builder.Key(this->Name().c_str());
    builder.Begin(Zilch::JsonType::Object);        
    // Serialize this entity's properties
    Object::SerializeByType(builder, interface->getState(), this, this->ZilchGetDerivedType());
    
    // Serialize all of its components
    for (auto& component : ComponentsContainer) {
      component->Serialize(builder);
    }

    builder.End();

  }

  /**************************************************************************/
  /*!
  @brief Deserializes an entity
  @param properties A pointer to the object containing the properties.
  @note  This will deserialize the entity's properties, then its components.
  */
  /**************************************************************************/
  void Entity::Deserialize(Zilch::JsonValue * properties)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Archetype setter.
  */
  /**************************************************************************/
  void Entity::setArchetype(std::string archetypeName)
  {
    ArchetypeName = archetypeName;
  }

  /**************************************************************************/
  /*!
  @brief  Archetype getter.
  */
  /**************************************************************************/
  std::string Entity::getArchetype() const
  {
    return ArchetypeName;
  }

  /**************************************************************************/
  /*!
  @brief  Adds a component onto the entity by name.
  @param  name The name of the component.
  @bool   Whether to initialize the component.
  @return A pointer to the component.
  @todo   Do this dynamically rather than using string->class mapping.
  */
  /**************************************************************************/
  ComponentPtr Entity::AddComponentByName(std::string & name, bool initialize)
  {    
    auto Factory = Daisy->getSystem<Systems::Factory>();
    auto component = Factory->CreateComponentByName(name, *this);
    // If the component could not be constructed... 
    if (component == nullptr) {
      DCTrace << ObjectName << "::AddComponentByName - " << name << " could not be added!\n";
      return nullptr;
    }    
    ComponentsContainer.emplace_back(std::move(component));
    //ComponentsContainer.emplace_back(std::move(Factory->CreateComponentByName(name, *this)));
    DCTrace << ObjectName << "::AddComponentByName - " << name << " has been added!\n";
    // Initialize the component if need be
    if (initialize)
      ComponentsContainer.back().get()->Initialize();
   // Returns a pointer to the just-added component
    return ComponentsContainer.back().get();
  }

  /**************************************************************************/
  /*!
  @brief  Adds a component onto the entity by Zilch::BoundType. This is a
          pointer data about the bound component.
  @param  boundType A pointer to the component's BoundType.
  */
  /**************************************************************************/
  bool Entity::AddComponentByType(Zilch::BoundType * boundType, bool initialize)
  {    
    auto Factory = Daisy->getSystem<Systems::Factory>();
    ComponentsContainer.emplace_back(std::move(Factory->CreateComponentByType(boundType, *this)));
    DCTrace << "Entity::AddComponentByType - " << std::string(boundType->Name.c_str()) << "\n";
    // Initialize the component if need be
    if (initialize)
      ComponentsContainer.back().get()->Initialize();
    return true;
  }

  /**************************************************************************/
  /*!
  @brief  Removes the component from the entity.
  @param  component A pointer to the component.
  */
  /**************************************************************************/
  void Entity::RemoveComponent(ComponentPtr component)
  {
    //ComponentsContainer.erase(std::remove(ComponentsContainer.begin(),
    //                          ComponentsContainer.end(), 
    //  [&](std::unique_ptr<Component> const& p) {
    //  return p.get() == component;
    //}),
    //                          ComponentsContainer.end());
  }

  /**************************************************************************/
  /*!
  @brief  Returns a reference to the container of all the components this
          Entity has.
  @return A reference to the container of all components.
  */
  /**************************************************************************/
  ComponentStrongVec * Entity::AllComponents()
  {
    //ComponentVec componentsContainer;
    //// Makes a copy of the container
    //for (auto &component : ComponentsContainer) {
    //  componentsContainer.push_back(component);
    //}
    return &ComponentsContainer;
  }

  #if(DCE_BINDING_OBJECT_CLASSES_INTERNALLY)
  ZilchDefineType(Entity, "Entity", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, Entity, "name", std::string);
    ZilchBindDestructor(builder, type, Entity);
  }
  #endif

}