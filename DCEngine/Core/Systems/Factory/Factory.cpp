#include "Factory.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Default constructor for the Factory system.
    */
    /**************************************************************************/
    Factory::Factory() : System(std::string("FactorySystem"), EnumeratedSystem::Factory) {
    }

    /**************************************************************************/
    /*!
    \brief  Initializes the Factory system.
    */
    /**************************************************************************/
    void Factory::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
        DCTrace << "Factory::Initialize \n";
    }

    /**************************************************************************/
    /*!
    \brief  Updates the factory system.
    */
    /**************************************************************************/
    void Factory::Update(float dt) {
      if (TRACE_UPDATE)
        DCTrace << "Factory::Update \n";

      // Destroy all objects on the to-be-deleted listf
      for (auto gameObj : gameObjsToBeDeleted) {
        
      }

    }

    /**************************************************************************/
    /*!
    \brief  Terminates the Factory system.
    */
    /**************************************************************************/
    void Factory::Terminate() {
        DCTrace << "Factory::Terminate \n";
    }

    /**************************************************************************/
    /*!
    \brief  Creates a game object with default components.
    \param  A reference to the space where the object will be constructed on.
    \param  Whether the GameObject should be initialized right away.
    \return A GameObject created on the space.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(std::string& name, Space& space, bool init) {
      // Create the GameObject
      GameObjectPtr gameObj(new GameObject(name, space, space.getGameSession()));
      gameObjVec.push_back(gameObj);
      // Create the Component, and add it to the GameObject
      ComponentPtr transform = ComponentPtr(new Transform(dynamic_cast<Entity&>(*gameObj)));
      ComponentContainer.push_back(transform);
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

    //// ? We do not want to create components by name..
    //ComponentPtr Factory::CreateComponent(const std::string & compName, bool init) {
    //  return ComponentPtr(new Transform())
    //}

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

    void Factory::DeleteGameObject(GameObjectPtr gameObj)
    {
      //delete gameObj;
    }
  }
}