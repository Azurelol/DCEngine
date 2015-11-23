/******************************************************************************/
/*!
@file  Level.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Level.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  Level constructor.
  */
  /**************************************************************************/
  Level::Level(std::string levelFile) : Resource(FileSystem::FileNoExtension(levelFile)),
                                                  SourceFile(levelFile) {
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

  /**************************************************************************/
  /*!
  @brief  Serializes the data definition of a level from a string back into
         the source file now that we have modified it.
  */
  /**************************************************************************/
  void Level::Save(std::string & serializedData)
  {
    SerializedData = serializedData;
    FileSystem::FileWriteString(SourceFile, SerializedData);
  }

  /**************************************************************************/
  /*!
  @brief  Deserializes the binary cache the serialized data definition of a
          level into a string that can be parsed by the engine's serializer.
  */
  /**************************************************************************/
  bool Level::Load()
  {
    if (FileSystem::FileReadToString(SourceFile, SerializedData)) {
      DCTrace << ObjectName << "::Level::Load - Loading from " << SourceFile <<  " succeeded \n";
      return true;
    }
    DCTrace << ObjectName << "::Level::Load -Loading from " << SourceFile << " failed \n";
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


