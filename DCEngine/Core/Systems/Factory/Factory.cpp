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
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Default constructor for the Factory system.
    */
    /**************************************************************************/
    Factory::Factory() : System("FactorySystem", EnumeratedSystem::Factory) {
    }

    /**************************************************************************/
    /*!
    @brief  Initializes the Factory system.
    @todo   Find another way to add the rebound components.
    */
    /**************************************************************************/
    void Factory::Initialize() {
      if (TRACE_ON && TRACE_INITIALIZE)
        DCTrace << "Factory::Initialize \n";
      
      // Construct the component factories for each core component type
      ConstructComponentFactoryMap();
      // Construct the component factories for 'Rebound' components
      ReboundComponentsAddToFactory();
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
      // Destroy all marked components
      DestroyComponents();
      // Destroy all marked spaces
      DestroySpaces();
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
    @param  name The name of the GameObject.
    @param  space A reference to the space where the object will be constructed on.
    @param  init Whether the GameObject should be initialized right away.
    @return A pointer to GameObject created on the space.
    @note   The factory owns a container of strong pointers of all active GameObjects.
            Everyone else just has raw pointers, which do not have ownership.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(std::string name, Space& space, bool init) {

      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject(name, space, *space.getGameSession())));
      auto gameObjPtr = ActiveGameObjects.back().get();
      gameObjPtr->AddComponentByName(std::string("Transform"));
      //gameObjPtr->AddComponent<Components::Transform>();
      // If the object needs to be initialized right away
      if (init)
        gameObjPtr->Initialize();
      return gameObjPtr;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a game object from an Archetype.
    @param  archetype The archetype from which to construct this object from.
    @param  space A reference to the space where the object will be constructed on.
    @param  init Whether the GameObject should be initialized right away.
    @return A GameObject created on the space.
    @todo   Refactor this so it uses some JSON interface class rather than
            .. this.
    */
    /**************************************************************************/
    GameObjectPtr Factory::CreateGameObject(ArchetypePtr archetype, Space & space, bool init)
    {
      // 1. Construct the GameObject
      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject("Object", space, *space.getGameSession())));
      auto gameObjPtr = ActiveGameObjects.back().get();
      // 2. Build it from serialized data
      BuildFromArchetype(gameObjPtr, archetype);


      //// Turn the string from file into JSON data
      //Zilch::CompilationErrors errors;
      //Zilch::JsonReader reader;
      //const Zilch::String what;
      //Zilch::JsonValue* archetypeData = reader.ReadIntoTreeFromString(errors,
      //                              archetype->Get().c_str(), what, nullptr);
      //
      //auto data = *archetypeData->OrderedMembers.data();

      return gameObjPtr;
    }


    /**************************************************************************/
    /*!
    @brief  Builds a GameObject from serialized data.
    @param  serializedData A pointer to the serialized data for the GameObject.
    @param  space A referene to the Space the GameObject will be created on/
    @return A pointer to the GameObject created on the space.
    @todo   Refactor how it's done..
    */
    /**************************************************************************/
    GameObjectPtr Factory::BuildGameObject(SerializedMember * data, Space & space)
    {      
      // 1. Construct the GameObject
      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject("Object", space, *space.getGameSession())));
      auto gameObject = ActiveGameObjects.back().get();
      // 2. Build it from serialized data
      BuildEntity(gameObject, data);

      return gameObject;
    }

    /**************************************************************************/
    /*!
    @brief  Builds an Entity from serialized data.
    @param  entity A pointer to the entity.
    @param  objectData A pointer to the serialized data for the Entity.
    @return A pointer to the GameObject created on the space.
    @todo   Refactor how it's done..
    */
    /**************************************************************************/
    EntityPtr Factory::BuildEntity(EntityPtr entity, SerializedMember* objectData)
    {      
      // 1. For every property... !!! CURRENTLY HARDCODED !!!
      auto name = objectData->Value->GetMember("Name")->AsString().c_str();
      entity->setObjectName(name);
      auto archetype = objectData->Value->GetMember("Archetype")->AsString().c_str();
      entity->setArchetype(archetype);

      auto gameObject = objectData->Value;
      for (auto property : gameObject->OrderedMembers.all()) {
        // Deserialize it
        //gameObjPtr->Deserialize(property->Value);
      }

      // 3. For each of its components..
      auto components = objectData->Value->GetMember("Components")->OrderedMembers.all();
      for (auto component : components) {
        // Construct the Component
        auto componentName = std::string(component->Key.c_str());
        auto componentPtr = entity->AddComponentByName(componentName);
        // If the component was successfully constructed..
        if (componentPtr) {
          // Deserialize it
          auto properties = component->Value;
          componentPtr->Deserialize(properties);
        }
      }

      return entity;
    }

    /**************************************************************************/
    /*!
    @brief  Builds an entity given serialized data.
    @param  entity A pointer to the entity.
    @param archetype A pointer to the archetype.
    */
    /**************************************************************************/
    void Factory::BuildFromArchetype(EntityPtr entity, ArchetypePtr archetype)
    {
      // 1. Get the Archetype data
      Zilch::CompilationErrors errors;
      Zilch::JsonReader reader;
      const Zilch::String what;
      Zilch::JsonValue* archetypeData = reader.ReadIntoTreeFromString(errors,
        archetype->Get().c_str(), what, nullptr);
      auto data = *archetypeData->OrderedMembers.data();
      // 2. Build the GameObject
      BuildEntity(entity, data);
    }

    /**************************************************************************/
    /*!
    @brief  Rebuilds an entity from its current archetype, removing all its
            components and remaking them anew from its archeetype.
    @param  entity A pointer to the entity.
    */
    /**************************************************************************/
    void Factory::RebuildFromArchetype(EntityPtr entity)
    {
      // If the entity has a transform component, save that data.
      TransformDataPair transformData;
      if (entity->HasComponent("Transform")) {
        transformData = entity->getComponent<Components::Transform>()->getTransformDataPair();
      }

      // 1. Remove all of the entities components
      entity->RemoveAllComponents();
      // 2. Grab the Archetype resource from the handle.
      auto archetype = Daisy->getSystem<Content>()->getArchetype(entity->getArchetype());
      // 3. Rebuild the object from its archetype
      BuildFromArchetype(entity, archetype);
      
      // 4. Set back its transform data.
      if (entity->HasComponent("Transform")) {
        auto transform = entity->getComponent<Components::Transform>();
        transform->setTranslation(transformData.second.Translation);
      }

    }

    /**************************************************************************/
    /*!
    @brief  Builds all the GameObjects from a level into a space.
    @param  level A pointer to the level resource.
    @param  space A reference to the Space the GameObject will be created on/
    @return A pointer to the GameObject created on the space.
    */
    /**************************************************************************/
    bool Factory::BuildFromLevel(LevelPtr level, Space & space)
    {
      // Turn the string from file into JSON data
      Zilch::CompilationErrors errors;
      Zilch::JsonReader levelReader;
      const Zilch::String levelAsJson;
      Zilch::JsonValue* levelData = levelReader.ReadIntoTreeFromString(errors, level->Get().c_str(), levelAsJson, nullptr);

      // If the data failed to load... 
      if (levelData == nullptr) {
        DCTrace << "Factory::BuildFromLevel - Failed to load level data from file! \n";
        return false;
      }
      
      DCTrace << "Factory::BuildFromLevel - Building GameObjects from Level: '" << level->Name() << "' \n";

      // 1. For every GameObject...
      auto gameObjects = levelData->GetMember("Level")->GetMember("GameObjects");      
      for (auto gameObjectValue : gameObjects->OrderedMembers.all()) {
        // 2. Build the GameObject
        auto gameObjectPtr = BuildGameObject(gameObjectValue, space);
        // 3. Add it to the space's container of active gameobjects
        space.AddObject(gameObjectPtr);
      }

      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Builds a level from a Space.
    @param  name The name for the level resource.
    @param  space A referene to the Space the Level will take objects from.
    @return A pointer to the created Level resource.
    */
    /**************************************************************************/
    LevelPtr Factory::BuildLevel(std::string level, Space & space)
    {
      // Create a builder object to build JSON
      Zilch::JsonBuilder levelBuilder;
      
      levelBuilder.Begin(Zilch::JsonType::Object); {
        // I. Level
        levelBuilder.Key("Level");
        levelBuilder.Begin(Zilch::JsonType::Object); {

          // 1. Name
          {
            levelBuilder.Key("Name");            
            Zilch::String levelName = FileSystem::FileNoExtension(level).c_str();            
            levelBuilder.Value(levelName);            
          }

          // 2. Space
          {
            //space.Serialize(levelBuilder);
          }        

          // 2. GameObjects
          {
            levelBuilder.Key("GameObjects");
            levelBuilder.Begin(Zilch::JsonType::Object);
            for (auto &gameObj : space.GameObjectContainer) {
              // Do not serialize the editor's camera
              if (gameObj->Name() == "EditorCamera")
                continue;
              gameObj->Serialize(levelBuilder);
            }
            levelBuilder.End();
          }
        } levelBuilder.End();

      } levelBuilder.End();   

      DCTrace << "Factory::BuildLevel - Saved level: " << level << " to file: data from file! \n";
      return LevelPtr(new Level(level, 
                                std::string(levelBuilder.ToString().c_str())) );
    }

    /**************************************************************************/
    /*!
    @brief  Builds an Archetype.
    @param  archetype The name of the Archetype.
    @param  gameObj A pointer to the GameObject.
    @return A pointer to the Archetype resource.
    */
    /**************************************************************************/
    ArchetypePtr Factory::BuildArchetype(std::string archetype ,GameObjectPtr gameObj)
    {
      // Create a builder object to build JSON
      Zilch::JsonBuilder archetypeBuilder;     
      archetypeBuilder.Begin(Zilch::JsonType::Object);
      {
        //archetypeBuilder.Key("GameObject");
        //archetypeBuilder.Begin(Zilch::JsonType::Object);
        //{
          gameObj->Serialize(archetypeBuilder);
        //}
        //archetypeBuilder.End();
      }      
      archetypeBuilder.End();
      
      //return std::string(archetypeBuilder.ToString().c_str());
      return ArchetypePtr( new Archetype(archetype,
                                         std::string(archetypeBuilder.ToString().c_str()) ));
    }

    /**************************************************************************/
    /*!
    @brief  Creates a Component by a given name.
    @param  name The name of the component.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentStrongPtr Factory::CreateComponentByName(const std::string & name, Entity & entity)
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
    @brief  Creates a Component by a given name through Zilch
    @param  name The name of the component.
    @param  entity A reference to the entity.
    @return Returns a component by strong pointer.
    */
    /**************************************************************************/
    ComponentHandle Factory::CreateComponentByNameFromZilch(const std::string & name, Entity & entity)
    {      
      auto state = Daisy->getSystem<Reflection>()->Handler()->GetState();
      Zilch::ExceptionReport report;

      // Get the component's BoundType
      auto boundType = Component::BoundType(name);
      // Allocate the component on the heap through Zilch
      auto componentHandle = state->AllocateHeapObject(boundType, report, Zilch::HeapFlags::ReferenceCounted);      
      // Call the component's constructor explicitly
      Zilch::Call ctorCall(boundType->Constructors[0], state);
      ctorCall.SetHandle(Zilch::Call::This, componentHandle);
      ctorCall.Set(0, entity);
      ctorCall.Invoke(report);
      // Return the handle to this component
      return componentHandle;
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
    @brief Marks the component for deletion on the next frame.
    @param component A reference to the component.
    */
    /**************************************************************************/
    void Factory::MarkComponent(Component & component)
    {
      DCTrace << "Factory::MarkComponent - " << component.getObjectName() << " has been marked for removal \n";
      ComponentsToBeDeleted.insert(ComponentPtr(&component));
    }

    /**************************************************************************/
    /*!
    @brief Destroys all marked components. This removes them from the entities
           they are attached to.    
    @todo  May have to do different logic if multiple components from the same
           object are asked to be removed on the same frame...
    */
    /**************************************************************************/
    void Factory::DestroyComponents()
    {
      for (auto& component : ComponentsToBeDeleted) {
        DCTrace << "Factory::DestroyComponents - Destroying: " << component->Name() << "\n";
        component->Owner()->RemoveComponentByName(component->Name());
      }
      ComponentsToBeDeleted.clear();

    }

    /**************************************************************************/
    /*!
    @brief Destroys all marked spaces.
    @todo  May not be working properly...
    */
    /**************************************************************************/
    void Factory::DestroySpaces()
    {
      for (auto& space : SpacesToBeDeleted) {
        space->GameSessionRef->RemoveSpace(space);        
      }
      SpacesToBeDeleted.clear();
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

    void Factory::MarkSpace(Space& space)
    {
      SpacesToBeDeleted.insert(SpacePtr(&space));
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

      // This is set back to default, from 'Space::LoadLevel'
      SoundInstance::StopOnDestroyed = false;

    }

    GameObjectPtr Factory::BuildAndSerialize(const std::string & fileName) {
      // Construct the object with defaults
      GameObjectPtr gameObj(new GameObject());
      // Open the input file
      return gameObj;
    }

  }
}