/******************************************************************************/
/*!
\file   Component.h
\author Allan Deutsch, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/

#include "Component.h"


#include "Entity.h" // EntityPtr
#include "Entities\Space.h"
#include "Entities\GameSession.h"

// Access to the engine for key_callback
#include "..\..\Core\Engine\Engine.h"

namespace DCEngine {

  // Static member variables
  unsigned int Component::ComponentsCreated = 0;
  unsigned int Component::ComponentsDestroyed = 0;
  std::string Component::ComponentLastCreated;
  std::string Component::ComponentLastDestroyed;
  // Enable diagnostics
  bool Component::DiagnosticsEnabled = true;

  /**************************************************************************/
  /*!
  @brief Component constructor.
  @param name The name of the Component class.
  @param owner A reference to the Entity that owns this component.
  */
  /**************************************************************************/
  Component::Component(std::string name, Entity& owner)
    : Object(name), ComponentID(ComponentsCreated++) {
    ObjectOwner = (Object*)&owner;

    // Set references
    SetReferences();

    if (DCE_TRACE_COMPONENT_CONSTRUCTOR) {
      DCTrace << ObjectName << "::Component - Constructor - "
        << "Owner: '" << ObjectOwner->Name()
        << "'\n";
    }

    // Diagnostics
    if (DiagnosticsEnabled)
      ComponentLastCreated = ObjectName;
  }

  /**************************************************************************/
  /*!
  @brief Component destructor.
  */
  /**************************************************************************/
  Component::~Component()
  {
    if (DCE_TRACE_COMPONENT_DESTRUCTOR)
      DCTrace << ObjectOwner->Name() << "::" << Name()
      << "::~Component - Destructor called! \n";
    ComponentsDestroyed++;

    // Deregister from all publishers
    for (auto publisher : ActiveDelegateHolders) {
      //publisher->Dis
      Daisy->Disconnect<Entity>(publisher, this);
    }


    if (DiagnosticsEnabled)
      ComponentLastDestroyed = ObjectName;
  }

  /**************************************************************************/
  /*!
  @brief Destroys the component at the beginning of the next frame.
  */
  /**************************************************************************/
  void Component::Destroy()
  {
    // Mark the component to be deleted by the factory on the next frame
    Daisy->getSystem<Systems::Factory>()->MarkComponent(*this);
  }

  /**************************************************************************/
  /*!
  @brief Serializes a Component.
  @param builder A reference to the JSON builder.
  @note  This will serialize the component and all its properties.
  */
  /**************************************************************************/
  void Component::Serialize(Zilch::JsonBuilder & builder)
  {
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();

    builder.Key(this->Name().c_str());
    builder.Begin(Zilch::JsonType::Object);
    SerializeByType(builder, interface->getState(), this, this->ZilchGetDerivedType());
    builder.End();
  }

  /**************************************************************************/
  /*!
  @brief Deserializes a Component.
  @param builder A pointer to the object containing the properties.
  @note  This will deserialize the Component's properties.
  */
  /**************************************************************************/
  void Component::Deserialize(Zilch::JsonValue * properties)
  {
    if (DCE_TRACE_COMPONENT_INITIALIZE)
      DCTrace << Owner()->Name() << "::" << ObjectName << "::Deserialize \n";
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    DeserializeByType(properties, interface->getState(), this, this->ZilchGetDerivedType());
  }

  /**************************************************************************/
  /*!
  @brief Checks whether the component exists among the list of active
         components.
  @param componentName The name of the component.
  @return Whether the component exists among the list of created components.
  */
  /**************************************************************************/
  bool Component::Exists(std::string componentName)
  {
    for (auto& component : AllComponents()) {
      if (std::string(component->Name.c_str()) == componentName)
        return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Returns a pointer to the Entity that owns this component.
  @return An entity pointer.
  */
  /**************************************************************************/
  Entity* Component::Owner() {
    return dynamic_cast<Entity*>(ObjectOwner);
  }

  /**************************************************************************/
  /*!
  @brief Adds a dependency to this component.
  @param componentName The component this component depends on.
  @return The success of the operation.
  */
  /**************************************************************************/
  //bool Component::AddDependency(std::string componentName)
  //{
  //  // Check if the component is a valid component name
  //  if (!Exists(componentName)) {
  //    DCTrace << "Component::AddDependency: '" << componentName << "' does not exist! \n";
  //    return false;
  //  }
  //  // Check that it hasn't been already added
  //  //if (Dependencies.find(componentName)
  //
  //  Dependencies.push_back(componentName);
  //}

  /**************************************************************************/
  /*!
  @brief  Checks whether this component's owner has all the components
          this component depends on.
  @return Whether this component's dependencies have been fulfilled.
  */
  /**************************************************************************/
  //bool Component::CheckForDependencies()
  //{
  //  // Look for every component dependency in this component's owner
  //  for (auto& dependency : Dependencies) {
  //    if (this->Owner()->HasComponent(dependency) == false)
  //      return false;
  //  }
  //  // All dependencies were found
  //  return true;
  //}

  /**************************************************************************/
  /*!
  @brief Sets the Owner reference for this component.
  */
  /**************************************************************************/
  void Component::SetReferences() {
    auto type = Owner()->Type();
    auto entity = dynamic_cast<Entity*>(Owner());

    // If the owner is a 'GameObject' entity
    if (type == EntityType::GameObject) {
      auto gameObj = (GameObject*)entity;
      SpaceRef = gameObj->GetSpace();
      GameSessionRef = SpaceRef->getGameSession();
    }

    // If the owner is a 'Space' entity
    if (type == EntityType::Space) {
      SpaceRef = dynamic_cast<Space*>(entity);
      GameSessionRef = SpaceRef->getGameSession();
    }

    // If the owner is a 'GameSession' entity
    if (type == EntityType::GameSession) {
      SpaceRef = NULL;
      GameSessionRef = dynamic_cast<GameSession*>(entity);
    }

    auto a = GameSessionRef;

  }

  /**************************************************************************/
  /*!
  @brief  Returns a container containing all the currently bound components.
  @return A container of all bound components.
  */
  /**************************************************************************/
  std::vector<Zilch::BoundType*> Component::AllComponents()
  {
    return Daisy->getSystem<Systems::Reflection>()->AllComponents();
  }

  /**************************************************************************/
  /*!
  @brief  Returns a Zilch::BoundType of the given component name.
  @param  The name of the component.
  @return A BoundType pointer.
  */
  /**************************************************************************/
  Zilch::BoundType * Component::BoundType(std::string componentName)
  {
    for (auto componentType : AllComponents()) {
      auto componentTypeName = std::string(componentType->Name.c_str());
      if (componentTypeName == componentName) {
        return componentType;
      }
    }
    return nullptr;
  }

}


