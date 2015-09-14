#include "Component.h"

#include "Entity.h" // EntityPtr
#include "Entities\Space.h"
#include "Entities\GameSession.h"

// Access to the engine for key_callback
//#include "..\..\Core\Engine\Engine.h"

namespace DCEngine {

  // All components can access the engine.
  //extern std::unique_ptr<Engine> Daisy;

  Component::Component(EnumeratedComponent type, BitfieldComponent mask, 
                       Entity& owner)
                        : Object("Component"), _type(type), _mask(mask) {
    owner_ = (Object*)&owner;
    // Set references
    SetReferences();

    if (TRACE_ON && TRACE_CONSTRUCTOR) {
      trace << _name << "::Component - Constructor - "
        << "Owner: '" << owner_->Name()
        << "'\n";
    }
  }

  Entity* Component::Owner() {
    return (Entity*)owner_;
  }

  void Component::SetReferences() {
    auto type = Owner()->Type();
    auto entity = (Entity*)Owner();

    // If the owner is a 'Space' entity
    if (type == EntityType::Space) {
      space_ = (Space*)entity;
      gamesession_ = (GameSession*)(space_->Owner());
    }
      
    // If the owner is a 'GameSession' entity
    if (type == EntityType::GameSession)
      gamesession_ = (GameSession*)Owner();

    trace << "lol";


  }

}


