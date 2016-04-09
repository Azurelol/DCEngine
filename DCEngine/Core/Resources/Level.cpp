/*****************************************************************************/
/*!
\file   Level.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  A level is a resource that stores a set of GameObjects that can be loaded
into a space.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Level.h"

#include "../Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  Level default constructor.
  */
  /**************************************************************************/
  Level::Level(std::string levelFile) : Resource("Level", FileSystem::FileNoExtension(levelFile), levelFile) {
    if (TRACE_CONSTRUCTOR)
      DCTrace << ObjectName << "::Level - Constructor \n";  
  }

  /**************************************************************************/
  /*!
  @brief  Level constructort that takes the data.
  */
  /**************************************************************************/
  Level::Level(std::string levelFile, std::string serializedData) : 
               Resource("Level", FileSystem::FileNoExtension(levelFile), levelFile) {
  
    // Saves the level data right away
    Save(serializedData);

    if (TRACE_CONSTRUCTOR)
      DCTrace << ObjectName << "::Level - Constructor \n";
  }

  /**************************************************************************/
  /*!
  @brief  Level destructor.
  */
  /**************************************************************************/
  Level::~Level() {

  }

  LevelPtr Level::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getLevel(name);
  }
  

  ///**************************************************************************/
  ///*!
  //@brief  Serializes the data definition of a level from a string back into
  //       the source file now that we have modified it.
  //*/
  ///**************************************************************************/
  //void Level::Save(std::string & serializedData)
  //{
  //  SerializedData = serializedData;
  //  FileSystem::FileWriteString(ResourcePath, SerializedData);
  //}

  /**************************************************************************/
  /*!
  @brief  Deserializes the binary cache the serialized data definition of a
          level into a string that can be parsed by the engine's serializer.
  */
  /**************************************************************************/
  bool Level::Load()
  {
    if (FileSystem::FileReadToString(ResourcePath, SerializedData)) {
      DCTrace << ObjectName << "::Level::Load - Loading from " << ResourcePath <<  " succeeded \n";
      return true;
    }
    DCTrace << ObjectName << "::Level::Load -Loading from " << ResourcePath << " failed \n";
    return false;
  }


  const std::string & Level::Get()
  {
    return SerializedData;
  }

  /**************************************************************************/
  /*!
  \brief  Creates an entity, adds it to the level. (Temporary until
          there's proper serialization and archetypes)
  \return A pointer to the entity that was added.
  */
  /**************************************************************************/
  void Level::AddGameObject(GameObjectPtr gameObject) {
    GameObjects.push_back(gameObject);
  }


}


