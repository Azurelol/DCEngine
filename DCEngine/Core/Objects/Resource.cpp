/*****************************************************************************/
/*!
\file   Resource.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  Resources are the source data of a game. They are assets such as textures,
sounds, levels, scripts. They are the resources found in a library window.

Each resource is assigned an unique id when created, allowing objects
and archetypes to refer to resources and not break their connection
even if the name of the resource is changed.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.


*/
/******************************************************************************/
#include "Resource.h"
#include "../Resources/ResourcesInclude.h"
#include "..\..\Core\Engine\Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  Resource constructor.
  @param  name The name of the Resource.
  @param  resourcePath The path of the file containing the resource on disk.
  */
  /**************************************************************************/
  Resource::Resource(std::string resourceType, std::string name, std::string resourcePath) 
                     : Object(name), ResourceType(resourceType), ResourcePath(resourcePath) {
  }

  /**************************************************************************/
  /*!
  @brief Serializes a Resource.
  @param builder A reference to the JSON builder.
  @note  This will serialize the resource and all its properties.
  */
  /**************************************************************************/
  void Resource::Serialize(Zilch::JsonBuilder & builder)
  {
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();

    builder.Key("Properties");
    builder.Begin(Zilch::JsonType::Object);
    SerializeByType(builder, interface->GetState(), this, this->ZilchGetDerivedType());
    builder.End();
  }

  /**************************************************************************/
  /*!
  @brief Deserializes a Resource.
  @param builder A reference to the JSON builder.
  @note  This will deserialize the resource and all its properties.
  */
  /**************************************************************************/
  void Resource::Deserialize(Zilch::JsonValue * properties)
  {
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    DeserializeByType(properties, interface->GetState(), this, this->ZilchGetDerivedType());
  }

  /**************************************************************************/
  /*!
  @brief  Builds a resource's serialized data.
  @return A string containing the serialized data of the resource.
  @note   After the resource's serialized data is built, it will be saved
          to file through the class' 'Save' method.
  */
  /**************************************************************************/
  std::string Resource::Build()
  {
    DCTrace << Name() << "::Resource::Build \n";
    Zilch::JsonBuilder builder;
    builder.Begin(Zilch::JsonType::Object);
    {
      builder.Key(ResourceType.c_str());
      builder.Begin(Zilch::JsonType::Object); {
        // Name
        {
          builder.Key("Name");
          Zilch::String resourceName = getObjectName().c_str();
          builder.Value(resourceName);
        }
        // Properties
        {
          this->Serialize(builder);
        }
      } builder.End(); // Resource
    } builder.End();

    auto serializedData = std::string(builder.ToString().c_str());
    DCTrace << serializedData;
    // Save the serialized data
    Save(serializedData);
    // Return it
    return serializedData;
  }

  /**************************************************************************/
  /*!
  @brief  Saves the Resource serialized data to file.
  @param  name The name of the Resource.
  @param  resourcePath The path of the file containing the resource on disk.
  */
  /**************************************************************************/
  void Resource::Save(std::string & serializedData)
  {
    SerializedData = serializedData;
    FileSystem::FileWriteString(ResourcePath, SerializedData);
  }

  /**************************************************************************/
  /*!
  @brief  Deserializes the binary cache the serialized data definition of a
          resource into a string that can be parsed by the engine's serializer.
  @note   The name of the resource is currently being set as its filename.
  */
  /**************************************************************************/
  bool Resource::Load()
  {
    // Attempt to read from the file on disk
    if (FileSystem::FileReadToString(ResourcePath, SerializedData)) {
      DCTrace << ObjectName << "::Level::Load - Loading from " << ResourcePath << " succeeded \n";

      // Attempt to parse the data as JSON
      Zilch::JsonReader reader;
      Zilch::CompilationErrors errors;
      Zilch::JsonValue* data = reader.ReadIntoTreeFromString(errors, SerializedData.c_str(), "Resource", nullptr);
      

      // If the data could not read
      if (!data) {
        DCTrace << getObjectName() << "::Resource::Load - The serialized data could not be read as valid JSON! \n";
        return false;
      }
      
      // Read and deserialize all the properties
      auto properties = data->GetMember(ResourceType.c_str())->GetMember("Properties");
      this->Deserialize(properties);
      return true;
    }

    // If the file could not be loaded
    DCTrace << ObjectName << "::" << ResourceType << "::Load - Loading from " << ResourcePath << " failed \n";
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Removes the resource from the Content system, as well as deleting
          its serialized file from disk.
  @return The success of the operation.
  */
  /**************************************************************************/
  bool Resource::Destroy()
  {
    return false;
  }

}