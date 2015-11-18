/******************************************************************************/
/*!
\file   Entity.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The x

*/
/******************************************************************************/
#include "Entity.h"

// Headers
#include "..\ComponentsInclude.h" // Entities need to know of componnets
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