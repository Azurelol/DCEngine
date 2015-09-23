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

      // Destroy all objects on the to-be-deleted list

    }
    void Factory::Terminate() {
      if (TRACE_UPDATE)
        trace << "Factory::Terminate \n";
    }

    /**************************************************************************/
    /*!
    \brief  Creates a game object with default components.
    \param  A reference to the space where the object will be constructed on.
    \param  Whether the GameObject should be initialized right away.
    \return If a match was found, a pointer to the component. If not, NULL.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(Space& space, bool init) {
      // Create the GameObject
      GameObjectPtr gameObj(new GameObject(std::string("GameObject"), space, space.getGameSession()));
      gameObjVec.push_back(gameObj);
      // Create the Component, and add it to the GameObject
      ComponentPtr transform = ComponentPtr(new Transform(dynamic_cast<Entity&>(*gameObj)));
      componentVec.push_back(transform);
      gameObj->AddComponent(transform);
      // Return the GameObject by shared_ptr.
      return gameObj;
    }

    GameObjectPtr Factory::CreateGameObject(const std::string & fileName, const Space& space, bool init) {
      GameObjectPtr gameObj = BuildAndSerialize(fileName);
      if (init)
        gameObj->Initialize();
      return gameObj;
    }

    ComponentPtr Factory::CreateComponent(const std::string & compName, bool init) {
      return ComponentPtr();
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