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
#include <BOOST/algorithm/string/replace.hpp>

// Engine
#include "../Engine/Engine.h"

namespace DCEngine {

#if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(ZilchScript, "ZilchScript", DCEngineCore, builder, type) {
  }
#endif

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
  @brief  Reads the string from.
  */
  /**************************************************************************/
  const std::string & ZilchScript::Read()
  {
    return this->SerializedData;
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

  /**************************************************************************/
  /*!
  @brief  Builds the default script code from a template file.
  @note   This will set up the script.
  @todo   In the future, perhaps take an argument for what template to use
          from the editor.
  */
  /**************************************************************************/
  bool ZilchScript::BuildDefaultFromTemplate()
  {
    // Load the script from the file
    std::string templatePath = "Core\\Assets\\Templates\\Component.Zilch";
    // If the template file could be read properly..
    if (FileSystem::FileReadToString(templatePath, SerializedData)) {
      // Replace the templated name
      auto name = Name();
      boost::replace_all(SerializedData, "Script", Name());
      //SerializedData.replace(SerializedData.begin(), SerializedData.begin() + SerializedData.find("Script"), Name().c_str() );
      // Save the script to file
      Save(SerializedData); // lol
      return true;
    } 
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Adds the script to Zilch so it generates the code for it. ?
  @todo   May be unnecessary.
  */
  /**************************************************************************/
  bool ZilchScript::IncludeScript()
  {   
    Daisy->getSystem<Systems::Reflection>()->Handler()->AddScript(SerializedData, getObjectName());
    return true;
  }

}