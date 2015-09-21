#include "PhysicsSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  PhysicsSpace::PhysicsSpace(Entity& owner) : Component(std::string("PhysicsSpace"), owner) {

  }

  void PhysicsSpace::Initialize() {
    //Daisy->getSystem<Systems::Audio>()->
  }
}