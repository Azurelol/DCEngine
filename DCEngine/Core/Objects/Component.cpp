#include "Component.h"

#include "Entity.h" // EntityPtr
#include "Entities\Space.h"
#include "Entities\GameSession.h"

// Access to the engine for key_callback
//#include "..\..\Core\Engine\Engine.h"

namespace DCEngine {

  // All components can access the engine.
  //extern std::unique_ptr<Engine> Daisy;

  Component::Component(std::string name, Entity& owner)
                        : Object(name) {
    owner_ = (Object*)&owner;

    // Set references
    SetReferences();

    if (TRACE_CONSTRUCTOR) {
      trace << ObjName << "::Component - Constructor - "
        << "Owner: '" << owner_->Name()
        << "'\n";
    }
  }

  Entity* Component::Owner() {
    return dynamic_cast<Entity*>(owner_);
  }

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


