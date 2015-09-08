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
    if (TRACE_ON && TRACE_CONSTRUCTOR) {
      trace << _name << "::Component - Constructor - "
        << "Owner: '" << owner_->Name()
        << "'\n";
    }
  }

  Entity* Component::Owner() {
    return (Entity*)owner_;
  }

}


