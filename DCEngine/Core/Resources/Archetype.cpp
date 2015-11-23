/******************************************************************************/
/*!
@file   Archtype.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Archetype.h"

namespace DCEngine {
  
  /**************************************************************************/
  /*!
  @brief  Archetype constructor.
  @param  archetypeFile The name of the archetype file.
  */
  /**************************************************************************/
  Archetype::Archetype(std::string archetypeFile) : Resource(FileSystem::FileNoExtension(archetypeFile)),
                                                             SourceFile(archetypeFile)
  {
    // Load the archetype from file immediately
    Load();
  }

  /**************************************************************************/
  /*!
  @brief  Archetype destructor.
  */
  /**************************************************************************/
  Archetype::~Archetype()
  {

  }

  /**************************************************************************/
  /*!
  @brief Serializes the data definition of an object from a string back into
         the source file now that we have modified it.
  */
  /**************************************************************************/
  void Archetype::Save(std::string& serializedData)
  {
    SerializedData = serializedData;
    FileSystem::FileWriteString(SourceFile, SerializedData);
  }

  /**************************************************************************/
  /*!
  @brief  Deserializes the binary cache the serialized data definition of an
          object into a string that can be parsed by the engine's serializer.
  */
  /**************************************************************************/
  void Archetype::Load()
  {
    FileSystem::FileReadToString(SourceFile, SerializedData);
  }

}