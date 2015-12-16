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
#include "ZilchScript.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  ZilchScript constructor.
  @param  scriptFile The path of the ZilchScript file.
  */
  /**************************************************************************/
  ZilchScript::ZilchScript(std::string scriptFile) : Resource("ZilchScript", 
                                                              FileSystem::FileNoExtension(scriptFile), 
                                                              scriptFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief  ZilchScript destructor.  
  */
  /**************************************************************************/
  ZilchScript::~ZilchScript()
  {
  }

  /**************************************************************************/
  /*!
  @brief Serializes the script now that is been modified.
  */
  /**************************************************************************/
  void ZilchScript::Save(std::string & serializedData)
  {
    SerializedData = serializedData;
    FileSystem::FileWriteString(ResourcePath, SerializedData);
  }

  /**************************************************************************/
  /*!
  @brief  Deserializes the script from file into theh SerializedData string.
  */
  /**************************************************************************/
  bool ZilchScript::Load()
  {
    {
      if (FileSystem::FileReadToString(ResourcePath, SerializedData))
      {
        return true;
      }
      DCTrace << getObjectName() << "::ZilchScript::Load - Failed to load! \n";
      return false;
    }
  }

  bool ZilchScript::GenerateCode()
  {
    return false;
  }

}