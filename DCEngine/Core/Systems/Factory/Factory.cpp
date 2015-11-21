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
      
      // Destroy all objects on the to-be-deleted list
      DestroyGameObjects();      
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
    @brief  Creates a game object with default components.
    @param  A reference to the space where the object will be constructed on.
    @param  Whether the GameObject should be initialized right away.
    @return A pointer to GameObject created on the space.
    @note   The factory owns a container of strong pointers of all active GameObjects.
            Everyone else just has raw pointers, which do not have ownership.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(std::string name, Space& space, bool init) {


      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject(name, space, space.getGameSession())));
      auto gameObjPtr = ActiveGameObjects.back().get();
      gameObjPtr->AddComponent<Transform>();
      //ComponentPtr transform = ComponentPtr(new Transform(dynamic_cast<Entity&>(*gameObjPtr)));
      //ActiveComponents.push_back(transform);
      //gameObjPtr->AddComponent<Transform>();
      // If the object needs to be initialized right away
      if (init)
        gameObjPtr->Initialize();
      return gameObjPtr;

      // Create the GameObject and own it by shared_ptr
      //GameObjectStrongPtr gameObj(new GameObject(name, space, space.getGameSession()));      
      //ActiveGameObjects.push_back(gameObj);
      //// Create a default Transform component, and add it to the GameObject
      //ComponentPtr transform = ComponentPtr(new Transform(dynamic_cast<Entity&>(*gameObj)));
      //ActiveComponents.push_back(transform);
      //gameObj->AddComponent(transform);
      //// If the object needs to be initialized right away
      //if (init)
      //  gameObj->Initialize();
      //// Return the GameObject by shared_ptr.
      //return gameObj.get();
      //return nullptr;
    }

    /**************************************************************************/
    /*!
    \brief  Creates a game object from an Archetype.
    \param  A reference to the space where the object will be constructed on.
    \param  Whether the GameObject should be initialized right away.
    \return A GameObject created on the space.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(ArchetypePtr archetype, Space & space, bool init)
    {
      return GameObjectPtr();
    }

    /**************************************************************************/
    /*!
    @brief  Marks the GameObject to be destroyed.
    @param  gameObj A reference to the GameObject.
    */
    /**************************************************************************/
    void Factory::MarkGameObject(GameObject& gameObj) {
      GameObjectsToBeDeleted.insert(GameObjectPtr(&gameObj));
    }
    /**************************************************************************/
    /*!
    @brief  Destroys all ActiveGameObjects in the GameObjectsToBeDeleted list.
    @note   
    */
    /**************************************************************************/
    void Factory::DestroyGameObjects()
    {      
      ActiveGameObjects.erase(
        std::remove_if( // Selectively remove elements in the second vector...
          ActiveGameObjects.begin(),
          ActiveGameObjects.end(),
          [&](std::unique_ptr<GameObject> const& p)
      {   // This predicate checks whether the element is contained
          // in the second vector of pointers to be removed...
        return std::find(
          GameObjectsToBeDeleted.cbegin(),
          GameObjectsToBeDeleted.cend(),
          p.get()
          ) != GameObjectsToBeDeleted.end();
      }),
        ActiveGameObjects.end()
        );

      GameObjectsToBeDeleted.clear();

    }

    GameObjectPtr Factory::BuildAndSerialize(const std::string & fileName) {
      // Construct the object with defaults
      GameObjectPtr gameObj(new GameObject());
      // Open the input file
      return gameObj;

    }

  }
}