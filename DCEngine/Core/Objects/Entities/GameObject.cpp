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

    type_ = EntityType::GameObject;
  }



  void GameObject::Serialize(Json::Value & root) {
    // Serialize primitives
    root["Name"] = _name;
  }

  void GameObject::Deserialize(Json::Value & root) {
    // Deserialize primitives
    _name = root.get("Name", "").asString();
  }

  Space* GameObject::GetSpace() {
    return space_;
  }

  GameSession * GameObject::GetGameSession() {
    return gamesession_;
  }

}