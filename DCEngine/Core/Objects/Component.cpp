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

    if (TRACE_CONSTRUCTOR) {
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

    if (DiagnosticsEnabled)
      ComponentLastDestroyed = ObjectName;
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
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    DeserializeByType(properties, interface->getState(), this, this->ZilchGetDerivedType());
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
      GameSessionRef = gameObj->GetGameSession();
    }

    // If the owner is a 'Space' entity
    if (type == EntityType::Space) {
      SpaceRef = (Space*)entity;
      GameSessionRef = (GameSession*)(SpaceRef->Owner());
    }
      
    // If the owner is a 'GameSession' entity
    if (type == EntityType::GameSession)
      SpaceRef = NULL;
      GameSessionRef = (GameSession*)Owner();
  }

}


