/*****************************************************************************/
/*!
@file   Archetype.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/14/2015
@brief  An archetype is a resource that contains the serialized data definition
of an object. The archetype stores a binary cache of the serialization
data and the source file for debugging and for archetype updating.
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
  Archetype::Archetype(std::string archetypeFile) : Resource("Archetype", FileSystem::FileNoExtension(archetypeFile), archetypeFile)                                                          
  {
    // Load the archetype from file immediately
    Load();
  }

  Archetype::Archetype(std::string archetypeFile, std::string serializedData) 
      : Resource("Archetype", FileSystem::FileNoExtension(archetypeFile), archetypeFile)
  {
    // Save the serialized data
    Save(serializedData);
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
   FileSystem::FileWriteString(ResourcePath, SerializedData);
 }

 /**************************************************************************/
 /*!
 @brief  Deserializes the binary cache the serialized data definition of an
         object into a string that can be parsed by the engine's serializer.
 */
 /**************************************************************************/
 bool Archetype::Load()
 {
   if (FileSystem::FileReadToString(ResourcePath, SerializedData))
   {
     return true;
   }
   DCTrace << getObjectName() << "::Archetype::Load - Failed to load! \n";
   return false;
 }

  const std::string & Archetype::Get()
  {
    return SerializedData;
  }


}