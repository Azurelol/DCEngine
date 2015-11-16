#include "Component.h"

#include "Entity.h" // EntityPtr
#include "Entities\Space.h"
#include "Entities\GameSession.h"

// Access to the engine for key_callback
//#include "..\..\Core\Engine\Engine.h"

namespace DCEngine {

  // Initialize static member variables
  unsigned int Component::ComponentsCreated = 0;
  unsigned int Component::ComponentsDestroyed = 0;

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


