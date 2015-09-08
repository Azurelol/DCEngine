#include "GameObject.h"

#include "Space.h"
#include "GameSession.h"

namespace DCEngine {
  GameObject::GameObject(std::string name, Space& space, GameSession& gamesession)
    : Entity(name), space_(&space), gamesession_(&gamesession) {

    if (TRACE_ON && TRACE_CONSTRUCTOR) {
      trace << _name << "::GameObject - Constructor - "
        << "', Space: '" << space_->Name()
        << "', GameSession '" << gamesession_->Name()
        << "\n";
    }


  }

}