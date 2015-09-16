#include "Physics.h"

namespace DCEngine {
  namespace Systems {

    Physics::Physics() : System(std::string("PhysicsSystem"), EnumeratedSystem::Physics) {
    }

    void Physics::Initialize() {
    }

    void Physics::Update(float dt) {
    }

    void Physics::Terminate() {
    }

  }

}