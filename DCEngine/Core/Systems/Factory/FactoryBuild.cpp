/*****************************************************************************/
/*!
@file   FactoryBuild.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/12/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Factory.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

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
      // 3. Prevent its serialized ID from being collided with
      GameObject::ActiveGameObjectIDs.insert(gameObject->GameObjectID);
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
      // 1. Deserialize specific properties
      if (auto gameObject = dynamic_cast<GameObjectPtr>(entity)) {
        gameObject->Deserialize(objectData->Value);
      }
      else if (auto space = dynamic_cast<SpacePtr>(entity))
        space->Deserialize(objectData->Value);

      //auto archetype = objectData->Value->GetMember("Archetype")->AsString().c_str();
      //entity->setArchetype(archetype);

      // 2. For each of its components..
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
      auto data = ArchetypeData(archetype);
      /*
      Zilch::CompilationErrors errors;
      Zilch::JsonReader reader;
      const Zilch::String what;
      Zilch::JsonValue* archetypeData = reader.ReadIntoTreeFromString(errors,
        archetype->Get().c_str(), what, nullptr);
      auto data = *archetypeData->OrderedMembers.data();*/
            
      // If the entity is currently constructed, do not construct a new one
      if (entity) {
        BuildEntity(entity, data);
        return;
      }
    }

    /**************************************************************************/
    /*!
    @brief Builds a GameObject from a given archetype.
    @param archetype A pointer to the archetype.
    @param  entity A pointer to the entity.
    */
    /**************************************************************************/
    GameObjectPtr Factory::BuildGameObjectFromArchetype(ArchetypePtr archetype, Space & space, bool init)
    {
      // 1. Grab the serialized data
      auto data = ArchetypeData(archetype);
      // 2. Construct the gameObject and all its children
      //BuildGameObjectAndChildren()
      // 1. Construct the GameObject
      ActiveGameObjects.emplace_back(GameObjectStrongPtr(new GameObject("Object", space, *space.getGameSession())));
      auto gameObjPtr = ActiveGameObjects.back().get();
      // 2. Build it from serialized data
      BuildFromArchetype(gameObjPtr, archetype);
      return gameObjPtr;
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
        transform->setRotation(transformData.second.Rotation);
        transform->setScale(transformData.second.Scale);
      }

    }

    /**************************************************************************/
    /*!
    @brief Reconstructs an entity by rebuilding its components.
    @param entity A pointer to the entity.
    */
    /**************************************************************************/
    void Factory::Rebuild(EntityBuildData data)
    {
      // Remove all its components
      data.first->RemoveAllComponents();
      // Rebuild all the components
      BuildFromArchetype(data.first, data.second);
    }

    /**************************************************************************/
    /*!
    @brief Builds all the GameObject and all its children, recursively.
    @param gameObjectData A pointer to the GameObject data.    
    @param space A reference to the Space the GameObject will be created on.
    @param parent A pointer to the parent of this GameObject.
    @return A pointer to the GameObject created on the space.
    */
    /**************************************************************************/
    void Factory::BuildGameObjectAndChildren(Zilch::JsonMember* gameObjectData, Space& space, GameObjectPtr parent) {
      // 1. Build the GameObject
      auto gameObject = BuildGameObject(gameObjectData, space);
      // 2. Add it to the space's container of active gameobjects
      space.AddObject(gameObject);
      // 3. Attach it to its parent
      gameObject->AttachTo(parent);
      // 4. Deserialize and build all its children      
      if (gameObjectData->Value->Members.count("Children")) {
        auto children = gameObjectData->Value->GetMember("Children")->OrderedMembers.all();
        for (auto child : children) {
          BuildGameObjectAndChildren(child, space, gameObject);
        }
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
      auto gameObjects = levelData->GetMember("Level")->GetMember("GameObjects");
      // 1. For every GameObject...
      for (auto gameObjectData : gameObjects->OrderedMembers.all()) {
        // Build the gameobject and all its children. We pass in null
        // since the root has no parent
        BuildGameObjectAndChildren(gameObjectData, space, nullptr);

        //// 2. Build the GameObject
        //auto gameObjectPtr = BuildGameObject(gameObjectData, space);
        //// 3. Add it to the space's container of active gameobjects
        //space.AddObject(gameObjectPtr);
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
            // For every level in the root
            for (auto &gameObj : space.GameObjectContainer) {
              // Do not serialize children from the root. They will be 
              // serialized recursively next...
              if (gameObj->Parent())
                continue;
              // Do not serialize the editor's camera
              if (gameObj->Name() == "EditorCamera")
                continue;
              // Serialize the GameObject and all its children
              gameObj->Serialize(levelBuilder);
            }
            levelBuilder.End();
          }
        } levelBuilder.End();

      } levelBuilder.End();

      DCTrace << "Factory::BuildLevel - Saved level: " << level << " to file: data from file! \n";
      return LevelPtr(new Level(level,
        std::string(levelBuilder.ToString().c_str())));
    }

    /**************************************************************************/
    /*!
    @brief  Builds an Archetype.
    @param  archetype The name of the Archetype.
    @param  gameObj A pointer to the GameObject.
    @return A pointer to the Archetype resource.
    */
    /**************************************************************************/
    ArchetypePtr Factory::BuildArchetype(std::string archetype, EntityPtr entity)
    {
      // Create a builder object to build JSON
      Zilch::JsonBuilder archetypeBuilder;
      archetypeBuilder.Begin(Zilch::JsonType::Object);
      {
        //archetypeBuilder.Key("GameObject");
        //archetypeBuilder.Begin(Zilch::JsonType::Object);
        //{
        if (auto gameObject = dynamic_cast<GameObjectPtr>(entity))
          gameObject->Serialize(archetypeBuilder);
        else if (auto space = dynamic_cast<SpacePtr>(entity))
          space->Serialize(archetypeBuilder);
        //}
        //archetypeBuilder.End();
      }
      archetypeBuilder.End();

      //return std::string(archetypeBuilder.ToString().c_str());
      return ArchetypePtr(new Archetype(archetype,
        std::string(archetypeBuilder.ToString().c_str())));
    }

  }
}