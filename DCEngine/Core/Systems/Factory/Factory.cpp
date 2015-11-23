/*****************************************************************************/
/*!
@file   Factory.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/20/2015
@brief  The Factory system handles the creation of all game objects and
components, from serialized data.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
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
      
      // Construct the component factories for each component type
      ConstructComponentFactoryMap();
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



    /*=====================*
    *  Object Construction *
    *=====================*/

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
      // If the object needs to be initialized right away
      if (init)
        gameObjPtr->Initialize();
      return gameObjPtr;
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
    @brief  Builds a GameObject from serialized data.
    @param  serializedData A pointer to the serialized data for the GameObject.
    @param  space A referene to the Space the GameObject will be created on/
    @return A pointer to the GameObject created on the space.
    */
    /**************************************************************************/
    GameObjectPtr Factory::BuildGameObject(SerializedMember* objectData, Space & space)
    {
      // 1. Get the name of the GameObject      
      auto name = std::string(objectData->Key.c_str());
      if (DCE_TRACE_FACTORY_GAMEOBJECT_CONSTRUCTION)
        DCTrace << "Factory::BuildGameObject - Building: '" << name << "' \n";
      // 2. Construct the GameObject
      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject(name, space, space.getGameSession())));
      auto gameObj = ActiveGameObjects.back().get();
      //auto gameObj = CreateGameObject(name, space, true);
      // 3. For each of its components..
      auto components = objectData->Value->OrderedMembers.all();
      for (auto component : components) {
        // Construct the Component
        auto componentName = std::string(component->Key.c_str());        
        auto componentPtr = gameObj->AddComponentByName(componentName, true);
        // If the component was successfully constructed..
        if (componentPtr) {
          // Deserialize it
          auto properties = component->Value;
          componentPtr->Deserialize(properties);
        }        

      }
      // 5. Add it to the space's container of active gameobjects
      space.AddObject(gameObj);
      // 6. Return a pointer to it
      return gameObj;
    }

    /**************************************************************************/
    /*!
    @brief  Builds all the GameObjects from a level into a space.
    @param  serializedData A pointer to the serialized data for the GameObject.
    @param  space A referene to the Space the GameObject will be created on/
    @return A pointer to the GameObject created on the space.
    */
    /**************************************************************************/
    bool Factory::BuildLevel(LevelPtr level, Space & space)
    {
      // Turn the string from file into JSON data
      Zilch::CompilationErrors errors;
      Zilch::JsonReader levelReader;
      const Zilch::String levelAsJson;
      Zilch::JsonValue* levelData = levelReader.ReadIntoTreeFromString(errors, level->Get().c_str(), levelAsJson, nullptr);

      // If the data failed to load... 
      if (levelData == nullptr) {
        DCTrace << "Factory::BuildLevel - Failed to load level data from file! \n";
        return false;
      }
      
      DCTrace << "Factory::BuildLevel - Building GameObjects from Level: '" << level->Name() << "' \n";

      // 1. For every GameObject...
      auto gameObjects = levelData->GetMember("Level")->GetMember("GameObjects");      
      for (auto gameObjectValue : gameObjects->OrderedMembers.all()) {
        // Build the GameObject
        BuildGameObject(gameObjectValue, space);
        //// Get the name of the GameObject
        //Zilch::String gameObjectName = gameObjectValue->Key;
        //// Build the GameObject: Construct, Deserialize it
        //auto gameObj = CreateGameObject(std::string(gameObjectName.c_str()), space, true);
        //auto components = gameObjectValue->Value->OrderedMembers.all();
        //for (auto component : components) {
        //  // Build its components: Construct, Deserialize each
        //  auto componentName = std::string(component->Key.c_str());
        //  gameObj->AddComponentByName(componentName, true);
        //  // Deserialize it
        //  //gameObj->a
        //}

        //// Deserialize it
        //// gameObj->Deserialize(gameObjectValue);
        //// Add it to the space's container of active gameobjects
        //space.AddObject(gameObj);

      }



        return true;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Component by a given name.
    @param  name The name of the component.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentStrongPtr Factory::CreateComponentByName(std::string & name, Entity & entity)
    { 
      // Look up the component by name among all boundtypes      
      for (auto componentType : Component::AllComponents()) {
        auto componentTypeName = std::string(componentType->Name.c_str());
        if (componentTypeName == name) {
          //DCTrace << "Factory::CreateComponentByName - " << componentTypeName << " ? " << name << " \n";
          return std::move(CreateComponentByType(componentType, entity));
        }
      }

      DCTrace << "Factory::CreateComponentByName - Could not find the component: '" << name << "' \n";
      return nullptr;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Component from a BoundType through the use of an instance
    of that component's factory.
    @param  boundType A pointer to the component's BoundType.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentStrongPtr Factory::CreateComponentByType(Zilch::BoundType * boundType, Entity & entity)
    {
      if (ComponentFactories[boundType].get() == nullptr)
        throw DCException("Factory::CreateComponentByType - Tried to construct '" + std::string(boundType->Name.c_str()) + "' that's not bound yet!");

      return ComponentFactories[boundType].get()->ConstructComponent(entity);
    }

    /**************************************************************************/
    /*!
    @brief Adds a component factory to the Factory system.
    @param type A pointer to the component BoundType.
    @param factory A strong pointer to the component factory.
    */
    /**************************************************************************/
    void Factory::AddComponentFactory(Zilch::BoundType * type, std::unique_ptr<AbstractComponentFactory> factory)
    {
      if (DCE_TRACE_FACTORY_COMPONENT_MAP) {
        DCTrace << "Factory::AddComponentFactory - Added: '" << type->Name.c_str() << "' \n";
      }
      ComponentFactories.emplace(type, std::move(factory));
    }

    /*===================*
    *  Object Management *
    *===================*/
    
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
      if (GameObjectsToBeDeleted.size() < 1)
        return;

      if (DCE_TRACE_GAMEOBJECT_DESTRUCTOR) {
        DCTrace << "Factory::DestroyGameObjects - Removing: \n";
        for (auto& gameObject : GameObjectsToBeDeleted) {
          DCTrace << " - " << gameObject->Name() << "\n";
        }
      }      

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