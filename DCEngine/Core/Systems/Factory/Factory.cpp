#include "Factory.h"

namespace DCEngine {
  namespace Systems {
    Factory::Factory() : System(std::string("FactorySystem"), EnumeratedSystem::Factory) {

    }
    void Factory::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
        trace << "Factory::Initialize \n";
    }
    void Factory::Update(float dt) {
      if (TRACE_UPDATE)
        trace << "Factory::Update \n";
    }
    void Factory::Terminate() {
      if (TRACE_UPDATE)
        trace << "Factory::Terminate \n";
    }

    GameObjectPtr Factory::CreateGameObject(const std::string & fileName, bool init) {
      GameObjectPtr gameObj = BuildAndSerialize(fileName);
      if (init)
        gameObj->Initialize();
      return gameObj;
    }

    void Factory::DestroyGameObject(GameObject& gameObj) {
      gameObjsToBeDeleted.insert(&gameObj);
    }

    void Factory::DestroyAllObjects() {

    }
    GameObjectPtr Factory::BuildAndSerialize(const std::string & fileName) {
      // Construct the object with defaults
      GameObjectPtr gameObj(new GameObject());
      // Open the input file


      return gameObj;

    }
  }
}