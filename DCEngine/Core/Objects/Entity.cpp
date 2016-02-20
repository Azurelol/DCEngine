/*/******************************************************************************/
/*!
@file   Entity.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/18/2015
@brief  The base object composition class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Entity.h"

// Headers
#include "Entities\Space.h"
#include "Entities\GameSession.h"

#include "../Engine/Engine.h" 



namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Entity constructor.
  */
  /**************************************************************************/
  Entity::Entity(std::string name) : Object("Entity"), ArchetypeName(""), Actions(*this) {
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
    // 1.A Factory-constructed components
    ComponentsContainer.clear();
    // 1.B Zilch-constructed components
    for (auto componentHandle : ComponentHandlesContainer)
      componentHandle.Delete();
    ComponentHandlesContainer.clear();

    // 2. Inform all observers of this entity's death
    InformObserversOfDeath();
  }

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

    // Initialize factory-created components
    for (auto &component : ComponentsContainer)
      component.get()->Initialize();
    // Initialize Zilch-created components
    for (auto& component : ComponentHandlesContainer) {
      reinterpret_cast<Component*>(component.Dereference())->Initialize();
    }
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

    // 1. Name
    //{
    //  builder.Key("Name");
    //  builder.Value(this->Name().c_str());
    //}

    // Serialize Object-specific properties
    Object::Serialize(builder);
    // Serialize Entity-specific properties
    SerializeByType(builder, interface->getState(), this, this->ZilchGetDerivedType()->BaseType);
    // Serialize all of its components
    builder.Key("Components");
    builder.Begin(Zilch::JsonType::Object);           
    // Factory-created components
    for (auto& component : ComponentsContainer) {
      component->Serialize(builder);
    }
    // Zilch-created components
    for (auto& component : ComponentHandlesContainer) {
      reinterpret_cast<Component*>(component.Dereference())->Serialize(builder);
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
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    DeserializeByType(properties, interface->getState(), this, this->ZilchGetDerivedType());
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
  ComponentPtr Entity::AddComponentByName(const std::string & name, bool initialize)
  { 
    // If the entity already has the component, do nothing.
    if (auto a = this->HasComponent(name)) {
      DCTrace << ObjectName << "::AddComponentByName - " << name << " is already present!\n";
      return nullptr;
    }

    #if (DCE_FACTORY_CONSTRUCT_COMPONENTS)
    auto Factory = Daisy->getSystem<Systems::Factory>();
    auto component = Factory->CreateComponentByName(name, *this);

    // If the component could not be constructed... 
    if (component == nullptr) {
      if (DCE_TRACE_COMPONENT_ADD)
        DCTrace << ObjectName << "::AddComponentByName - " << name << " could not be added!\n";
      return nullptr;
    }

    ComponentsContainer.emplace_back(std::move(component));
    if (DCE_TRACE_COMPONENT_ADD)
      DCTrace << ObjectName << "::AddComponentByName - " << name << " has been added!\n";

    // Initialize the component if need be
    if (initialize)
      ComponentsContainer.back().get()->Initialize();

    // Returns a pointer to the just-added component
    return ComponentsContainer.back().get();
    #else    
    auto componentHandle = Daisy->getSystem<Systems::Factory>()->CreateComponentByNameFromZilch(name, *this);
    // Add the component to the container
    ComponentHandlesContainer.push_back(componentHandle);
    // Initialize the component if need be
    auto componentPtr = reinterpret_cast<Component*>(componentHandle.Dereference());
    if (initialize)
      componentPtr->Initialize();
    return componentPtr;
    #endif


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
    if (DCE_TRACE_COMPONENT_ADD)
      DCTrace << "Entity::AddComponentByType - " << std::string(boundType->Name.c_str()) << "\n";
    // Initialize the component if need be
    if (initialize)
      ComponentsContainer.back().get()->Initialize();
    return true;
  }

  /**************************************************************************/
  /*!
  @brief  Checks whether the entity has the component already.
  @param  name The name of the component.
  @return Whether it has the component attached to it already.
  */
  /**************************************************************************/
  bool Entity::HasComponent(const std::string & name)
  {    
    for (auto& component : ComponentsContainer) {
      if (component->getObjectName() == name) {
        return true;
      }        
    }
    for (auto& component : ComponentHandlesContainer) {
      if (reinterpret_cast<Component*>(component.Dereference() )-> getObjectName() == name) {
        return true;
      }
    }


   // DCTrace << ObjectName << "::HasComponent - '" << name
   //   << "' is not present. \n";
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Removes the specified component from the entity.
  @param  component The name of the component
  @todo   Check for dependencies before removing the component.
  */
  /**************************************************************************/
  void Entity::RemoveComponentByName(std::string  componentName)
  {
    // Factory-owned components
    for (auto& component : ComponentsContainer) {
      if (component->getObjectName() == componentName) {
        DCTrace << "Entity::RemoveComponentByName - Removing " << componentName <<  "\n";
        // Check for dependencies
        
        // Remove it
        std::swap(component, ComponentsContainer.back());
        ComponentsContainer.pop_back();
        return;
      }
    }
    // Zilch-owned components
    for (auto& componentHandle : ComponentHandlesContainer) {
      auto component = reinterpret_cast<Component*>(componentHandle.Dereference());
      if (component->getObjectName() == componentName) {
        DCTrace << "Entity::RemoveComponentByName - Removing " << componentName << "\n";
        // Check for dependencies

        // Ask Zilch to delete the component 
        componentHandle.Delete();
        // Remove it
        std::swap(componentHandle, ComponentHandlesContainer.back());
        ComponentHandlesContainer.pop_back();
        return;
      }
    }

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
  @brief  Returns a container of pointers to the components this entity owns.
  @return A container of component pointers.
  @todo   Perhaps just pass the container of handles since this means
          this container is being constructed every frame.
  */
  /**************************************************************************/
  ComponentVec Entity::AllComponents()
  {
    // Fill the container of pointers to the components that this entity owns
    ComponentVec components;
    for (auto& component : ComponentsContainer) {
      components.push_back(component.get());
    }
    for (auto componentHandle : ComponentHandlesContainer) {
      components.push_back(reinterpret_cast<Component*>(componentHandle.Dereference()));
    }
    return components;
  }

  /**************************************************************************/
  /*!
  @brief Moves a component to the back of the entity's components container.
  @param component A reference to the component.
  */
  /**************************************************************************/
  void Entity::SwapToBack(ComponentPtr component)
  {
    auto handle = GetComponentHandle(component);
    auto handleIter = std::find(ComponentHandlesContainer.begin(), ComponentHandlesContainer.end(), handle);
    // Shift every element on the way to the back
    std::rotate(handleIter, handleIter + 1, ComponentHandlesContainer.end());
  }

  /**************************************************************************/
  /*!
  @brief  Returns a reference to the container of all the components this
          Entity has.
  @return A reference to the container of all components.
  */
  /**************************************************************************/
  //ComponentStrongVec * Entity::AllComponents()
  //{
  //  return &ComponentsContainer;
  //}



  /**************************************************************************/
  /*!
  @brief Returns a handle to the component given a pointer to it.
  @param component A reference to the component.
  @return A handle to the component.
  */
  /**************************************************************************/
  ComponentHandle Entity::GetComponentHandle(ComponentPtr component)
  {
    // Find the handle to this component since we internally we have a
    // container of handles to it since Zilch is managing them.
    ComponentHandle componentHandle;
    for (auto& handle : ComponentHandlesContainer) {
      // Once the component has been found
      if (reinterpret_cast<Component*>(handle.Dereference())->Name() == component->Name()) {
        componentHandle = handle;
        return componentHandle;
      }
    }
  }

  /**************************************************************************/
  /*!
  @brief  Informs all active observers of this entity's death. This allows
          them to remove this entity from their list of active delegate holders.
          Otherwise, CRASH!
  */
  /**************************************************************************/
  void Entity::InformObserversOfDeath()
  {
    // For every event in the map of events this entity is receiving...
    for (auto& event : ObserverRegistry) {
      // For every delegate in the list of delegates for each event... 
      for (auto it = event.second.begin(); it != event.second.end(); ++it) {        
        
        // Component: Inform each observer this object is being destroyed
        auto observer = (*it)->GetObserver();
        if (!observer)
          continue;

        if (auto component = dynamic_cast<ComponentPtr>(observer)) {          
          // Look in its container of active delegates for the pointer to this
          // entity, then remove it.
          for (auto& publisher : component->ActiveDelegateHolders) {
            if (this == publisher) {
              std::swap(publisher, component->ActiveDelegateHolders.back());
              component->ActiveDelegateHolders.pop_back();
              break;
            }
          }
        }

      }
    }

  }

  #if(DCE_BINDING_OBJECT_CLASSES_INTERNALLY)
  ZilchDefineType(Entity, "Entity", DCEngineCore, builder, type) {
    ZilchBindConstructor(builder, type, Entity, "name", std::string);
    ZilchBindDestructor(builder, type, Entity);
  }
  #endif

}